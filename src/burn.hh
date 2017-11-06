<?hh

require_once __DIR__.'/../vendor/hh_autoload.php';

$SELF_URL = 'http://localhost/burn/src/'.$argv[0];
// $SELF_URL = 'http://web3.dev.itn-bo.hhvm.dyninno.net:8080/eshchekochikhin/public_html/burn/src/'.$argv[0];
$DUMMY_URL = 'http://localhost';

function send(string $to): void
{
    $curl = curl_init();
    curl_setopt($curl, CURLOPT_URL, $to);
    $out = \HH\Asio\join(\HH\Asio\curl_exec($curl));
    $result =  $out ? substr($out, 0, 100) : curl_error($curl);
    curl_close($curl);

    print date("H-i-s").': '.$result.PHP_EOL;
}

$argv[1]
    ? \HH\Lib\Vec\range(1, $argv[1])
        |> \HH\Lib\Vec\map($$, $_ ==> send($SELF_URL)) && die()
    : send($DUMMY_URL);
