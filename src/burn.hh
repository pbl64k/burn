<?hh

$SELF_URL = 'http://127.0.0.1:8080/debug/tst/burn/src/'.$argv[0];
$DUMMY_URL = 'http://localhost';

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

if (isset($argv[1]) && isset($argv[2])) {
    array_map(
        $_ ==> \HH\Asio\join(\HH\Asio\v(array_map($_ ==> send($SELF_URL), range(1, intval($argv[2]))))),
        range(1, intval($argv[1])));
} else {
    \HH\Asio\join(send($DUMMY_URL));
}
