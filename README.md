# hhvm curl stress test

How:

1. point `$SELF_URL` (src/burn.hh:3) to itself (should be accessible via web).
2. `hhvm src/burn.hh 1000 50` -- the first argument controls the number of attempts, while the second one determines the number of parallel requests. Particulars may vary.

What:

Few seconds later hhvm service is dead?
