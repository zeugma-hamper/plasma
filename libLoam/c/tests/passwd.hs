#!/usr/bin/runhaskell

-- http://blog.julipedia.org/2006/08/split-function-in-haskell.html
mysplit :: String -> Char -> [String]
mysplit [] delim = [""]
mysplit (c:cs) delim
   | c == delim = "" : rest
   | otherwise = (c : head rest) : tail rest
   where
       rest = mysplit cs delim

processLine :: String -> String
processLine s =
  let fields = mysplit s ':'
      user = fields !! 0
      uid = fields !! 2
  in "  if (0 == strcmp (s, \"" ++ user ++
     "\")) return OB_CONST_I64 (" ++ uid ++ ");"

processFile :: String -> String
processFile s =
  let l = lines s
      changed = map processLine l
  in unlines changed

main = interact processFile
