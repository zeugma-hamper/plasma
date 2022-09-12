#!/usr/bin/runhaskell

{-
Run this program in this directory with no arguments to modify ob-log.c
such that the text in ob-log.h between "<!-- begin usage -->" and
"<!-- end usage -->" will get converted to a C string and inserted into
ob-log.c between "BEGIN AUTO-GENERATED (DO NOT EDIT)" and "END AUTO-GENERATED"
-}

import Data.List
import System.IO

replacer :: Char -> Char -> (Char, String)
replacer nxt c =
  let s = case (nxt, c) of
        ('<', '\\') -> ""       -- undo quoting that was necessary for Doxygen
        ('>', '\\') -> ""
        ('=', '?')  -> "?\" \"" -- avoid trigraph madness!
        (_, '"')    -> "\\\""   -- quote the quote character
        (_, '\\')   -> "\\\\"   -- and the backslash
        _ -> [c]
  in (c, s)

formatLine :: String -> String
formatLine =
  addQuotes . concat . snd . mapAccumR replacer ' '
  where addQuotes s = "  \"" ++ s ++ "\\n\""

stripStars :: [String] -> [String]
stripStars = map (drop 3)

trimBlankLines :: [String] -> [String]
trimBlankLines = foo . foo
  where foo = dropWhile null . reverse

extractLines :: [String] -> [String]
extractLines = takeWhile notEnd . tail . dropWhile notBegin
  where notBegin = not . isInfixOf "<!-- begin usage -->"
        notEnd = not . isInfixOf "<!-- end usage -->"

processHeader :: String -> [String]
processHeader = semi . f . lines
  where f = formatLines . trimBlankLines . stripStars . extractLines
        formatLines = map formatLine
        semi = reverse . semi' . reverse
        semi' (x:xs) = (x ++ ";"):xs

replaceGenerated :: [String] -> [String] -> [String]
replaceGenerated source generated =
  let beginStr = "// BEGIN AUTO-GENERATED (DO NOT EDIT)"
      endStr = "// END AUTO-GENERATED"
      before = takeWhile (not . (== beginStr)) source
      after = dropWhile (not . (== endStr)) source
  in before ++ [beginStr] ++ generated ++ after

main = do
  hHeader <- openFile "ob-log.h" ReadMode
  header <- hGetContents hHeader
  hSource <- openFile "ob-log.c" ReadMode
  source <- hGetContents hSource
  let newSrc = unlines $ replaceGenerated (lines source) $ processHeader header
  seq (length newSrc) $ do
    hClose hSource
    hClose hHeader
    writeFile "ob-log.c" newSrc
