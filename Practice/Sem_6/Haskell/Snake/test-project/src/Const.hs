module Const
    (
        leftDown,
        upRight,
        takeLen,
        size,
        snakeCollor,
        appleCollor,
        edgeCollor,
        bgColor
    ) where

import Graphics.Gloss

leftDown :: Int
leftDown = -10

upRight :: Int
upRight = 10
 
takeLen :: Int
takeLen = 2000

size :: Float
size = 30

snakeCollor :: Color
snakeCollor = blue

appleCollor :: Color
appleCollor = red

edgeCollor :: Color
edgeCollor = black

bgColor :: Color
bgColor = white