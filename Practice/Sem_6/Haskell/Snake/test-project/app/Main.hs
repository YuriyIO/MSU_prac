module Main (main) where


import System.Random
import Graphics.Gloss
import Graphics.Gloss.Interface.Pure.Game()

import Const (bgColor)
import Rules (steps, snakeInit, event, prossGame)
import Show (window, drawGame)


 
main :: IO ()
main = do
  gen <- getStdGen
  play
    window
    bgColor
    steps
    (snakeInit gen)
    drawGame
    event
    prossGame