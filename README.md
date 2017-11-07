# burn
hhvm curl crash test

How:

1. `composer install`
2. point `$SELF_URL` (in src/burn.hh) to itself (should be accessible via web)
3.`./run10x1000.sh`

What:

Few seconds later hhvm service is dead?