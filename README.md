# hhvm curl stress test

## Running:

`hhvm src/burn.hh http://127.0.0.1:8080/debug/tst/burn/src/burn.hh http://localhost 100 100`

The first argument should be the URL of burn.hh accessible through http.
The second argument is an arbitrary webpage to be retrieved by the sevice (here it's just the default Apache page).
The third argument controls the total number of attempts.
The fourth argument control the total number of parallel requests.

Particulars needed to cause the crash may vary.

## Effects:

After a few attempts hhvm serving requests through proxygen:

- either becomes unresponsive and hangs with 100% cpu usage
- or segfaults

