<?hh

async function send(string $to): Awaitable<string>
{
    $curl = curl_init();
    curl_setopt($curl, CURLOPT_URL, $to);
    $out = await \HH\Asio\curl_exec($curl);
    $result =  $out ? substr($out, 0, 100) : curl_error($curl);
    curl_close($curl);

    print date('H-i-s').': '.$result.PHP_EOL;

    return $result;
}

if (isset($argv[1]) && isset($argv[2] && isset($argv[3]) && isset($argv[4]))) {
    $endpoint = $argv[1];
    $remoteService = $argv[2];
    $attempts = $argv[3];
    $threads = $argv[4];

    array_map(
        $_ ==>
            \HH\Asio\join(\HH\Asio\v(
                array_map(
                    $_ ==> send($endpoint.'?'.urlencode($remoteService)),
                    range(1, intval($threads))))),
        range(1, intval($attempts)));
} elseif (isset($_SERVER['QUERY_STRING'])) {
    \HH\Asio\join(send(urldecode($_SERVER['QUERY_STRING'])));
}
