<?hh

async function send(string $to): Awaitable<string>
{
    print('Initiating request to [' . $to . ']...'."\n");

    $curl = curl_init();

    curl_setopt($curl, CURLOPT_URL, $to);
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

    $mh = curl_multi_init();

    curl_multi_add_handle($mh, $curl);

    $active = -1;

    do {
        $ret = curl_multi_exec($mh, $active);
    } while ($ret == CURLM_CALL_MULTI_PERFORM);

    while ($active && $ret == CURLM_OK) {
        $flag = await curl_multi_await($mh, 0.0);

        if ($flag === -1) {
            await \HH\Asio\usleep(100);
        }

        do {
            $ret = curl_multi_exec($mh, $active);
        } while ($ret == CURLM_CALL_MULTI_PERFORM);
    }

    $out = curl_multi_getcontent($curl);

    $result =  $out ? substr($out, 0, 100) : curl_error($curl);

    curl_multi_remove_handle($mh, $curl);

    curl_close($curl);

    curl_multi_close($mh);

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
    \HH\Asio\join(send(urldecode($_SERVER['QUERY_STRING'])));
} else {
    \HH\Asio\join(send('http://localhost'));
}

