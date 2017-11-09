<?hh

async function send(string $to): Awaitable<string>
{
    print('Initiating request to [' . $to . ']...'."\n");

    $curl = curl_init();
    curl_setopt($curl, CURLOPT_URL, $to);
    $out = await \HH\Asio\curl_exec($curl);
    $result =  $out ? substr($out, 0, 100) : curl_error($curl);
    curl_close($curl);

    print(date('H-i-s') . ': ' . $result . PHP_EOL);

    return $result;
}

async function sync_send(string $to): Awaitable<string>
{
    print('Initiating request to [' . $to . ']...'."\n");

    $curl = curl_init();
    curl_setopt($curl, CURLOPT_URL, $to);
    $out = curl_exec($curl);
    $result =  $out ? substr($out, 0, 100) : curl_error($curl);
    curl_close($curl);

    print(date('H-i-s') . ': ' . $result . PHP_EOL);

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
    \HH\Asio\join(sync_send(urldecode($_SERVER['QUERY_STRING'])));
}
