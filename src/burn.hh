<?hh

async function send(string $to): Awaitable<string>
{
    print('Initiating request to [' . $to . ']...'."\n");

    $curl = curl_init();
    curl_setopt($curl, CURLOPT_URL, $to);
    $out = await \HH\Asio\curl_exec($curl);
    $result =  $out ? substr($out, 0, 300) : curl_error($curl);
    curl_close($curl);

    print(date('H-i-s') . ': ' . $result . PHP_EOL);

    return $result;
}

async function send_file(string $to): Awaitable<string>
{
    print('Initiating (fake) request to [' . $to . ']... (Actually, just reading a local file.)'."\n");

    $pipes = [];
    
    proc_open('cat ' . __DIR__ . '/bork.txt', [1 => ['pipe', 'w']], $pipes);

    $fh = $pipes[1];

    $st = await stream_await($fh, STREAM_AWAIT_READ | STREAM_AWAIT_WRITE);
    if ($st === STREAM_AWAIT_READY) {
        $result = stream_get_contents($fh);
    } else {
        $result = 'Failure.';
    }
    fclose($fh);

    print(date('H-i-s') . ': [' . $result . ']' . PHP_EOL);

    return $result;
}

// CLI remote service invoker
if (isset($argv[1]) && isset($argv[2]) && isset($argv[3]) && isset($argv[4])) {
    $endpoint = $argv[1];
    $remoteService = $argv[2];
    $attempts = intval($argv[3]);
    $threads = intval($argv[4]);

    print('Working: ' . strval($attempts) . ' attempt(s) using ' . strval($threads) . ' threads to [' . $endpoint . ']...' . PHP_EOL);

    array_map(
        $_ ==>
            \HH\Asio\join(\HH\Asio\v(
                array_map(
                    $_ ==> send($endpoint.'?'.urlencode($remoteService)),
                    range(1, $threads)))),
        range(1, $attempts));
// remote service that retrieves the remote resource specified by the URL in the QUERY_STRING
} elseif (isset($_SERVER['QUERY_STRING'])) {
    //print(file_get_contents(__DIR__ . '/bork.txt'));
    \HH\Asio\join(send_file(urldecode($_SERVER['QUERY_STRING'])));
}
