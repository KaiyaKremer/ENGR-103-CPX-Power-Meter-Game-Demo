# **Power Meter Game**

**Console:** Arduino CPX  
**Author:** Kaiya Kremer

---

## **Description:**
This Arduino CPX game is a mini-game where the player must stop a moving blue LED when it is within a green target zone. The game has two button inputs and uses LED, voice, and melodical feedback. The game also features a switch for turning the game on/off and resetting it.

## **Inputs:**
- **Right Button:** Stop the Blue LED.
- **Left Button:** Instantly progress to the next level.
- **Switch:** Turn the game on/off and reset it.

## **Outputs:**
- **Rainbow LEDs:** Display a rainbow effect at the start of each level.
- **Blue Moving LED:** Indicates the position of the blue LED, which the player must stop.
- **Green LED Target Zone:** Indicates the area where the blue LED should be stopped.
- **Voice Output:** Provides audio feedback upon completing the game.
- **Sound Output:** Plays success tune upon completing the game.

## **Game Rules:**
- The game starts at level 1 and progresses to level 10.
- The player must hit the right button to stop the blue LED within the green target zone to progress to the next level.
- Failure to stop the blue LED within the green target zone results in game over.
- The player must beat level 10 to win the game.

## **Notes:**
- The green target zone decreases as the level increases.
- The left button can be used to skip to the next level.
- The game can be turned on/off using the switch.
