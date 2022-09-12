#!/bin/sh
set -e

usage() {
    cat <<_EOF_
Usage:
 '$0 save tmpdir [topdir]' at start of configure pass
 '$0 test tmpdir [topdir]' at end of test pass
_EOF_
}

case "$2" in
"") usage; exit 1;;
*) tmpdir="$2";;
esac

case "$3" in
"") ;;
*)  cd "$3";;
esac

export LANG=C

case "$1" in
"save")
    git ls-files --others --exclude-standard > "$tmpdir"/pre-config-git-dirt.txt
    ;;
"test")
    git ls-files --others --exclude-standard > "$tmpdir"/post-config-git-dirt.txt
    if diff -u "$tmpdir"/pre-config-git-dirt.txt "$tmpdir"/post-config-git-dirt.txt |
       grep -v subprojects/ |
       grep '^\+[^+]'
    then
        cat <<"_EOF_"

 _______________________________________
/ EDROPPINGS: Untracked files are       \
| present. For source files, did you    |
| forget to do 'git add'? For generated |
| files, did you forget to update       |
\ '.gitignore'?                         /
 ---------------------------------------
\                             .       .
 \                           / `.   .' "
  \                  .---.  <    > <    >  .---.
   \                 |    \  \ - ~ ~ - /  /    |
    \    _____          ..-~             ~-..-~
        |     |   \~~~\.'                    `./~~~/
       ---------   \__/   Mr. Git              \__/
      .'  O    \     /               /       \  "
     (_____,    `._.'               |         }  \/~~~/
      `----.          /       }     |        /    \__/
            `-.      |       /      |       /      `. ,~~|
                ~-.__|      /_ - ~ ^|      /- _      `..-'
                     |     /        |     /     ~-.     `-. _  _  _
                     |_____|        |_____|         ~ - . _ _ _ _ _>

_EOF_
        exit 1
    fi
    ;;
-h|--help)
    usage
    ;;
*)
    usage
    exit 1
    ;;
esac
