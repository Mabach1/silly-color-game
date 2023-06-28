# CLI color game

purpose of this tiny project was to test out a bit the possibilities of my C codebase as a tool for making CLI applications.

### How to play

The app will randomly choose four different colors from the Color eunm and you have to guess them! (Plan for the future is to display all the avalible colors every time the player is supposed to choose but for now just remember RGB + CMY + white and black)

After your decision is made, the app will tell you how many of your colors matches and how many are in the correct position like this:

```
+----------------------------+
| white magenta yellow red   | correct: 0, matched: 4
+----------------------------+
```
and every time you're asked to guess, you have, on display all your previous guesses: 
```
+----------------------------+
| white magenta yellow red   | correct: 0, matched: 4 
+----------------------------+
| cyan black red green       | correct: 1, matched: 0 
+----------------------------+
Your guess(4 colors): 
```

To guess you simply type your four colors into the terminal, seperated by space like this:
```
+----------------------------+
| white magenta yellow red   | correct: 0, matched: 4
+----------------------------+
| cyan black red green       | correct: 1, matched: 0
+----------------------------+
| magenta yellow red white   | correct: 2, matched: 2
+---------------------------:-+
Your guess(4 colors): magenta white red yellow
```

After hitting enter, the app will alert you if your input is incorrect, e.g. you inputed duplicate color or the color you trying to input can't be recognized.

You have **eight** attempts to guess the color combination and regardless of your success, a message, together with the correct color combination, will be displayed.

```
 You loss :(

Correct combination: yellow magenta blue green
```