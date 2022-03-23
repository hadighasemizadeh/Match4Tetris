# Match4Tetris

# Game Architecture
	•	MVC
# Matching Algorithm
	•	Breath-First search
# Game solution
	•	The game solution must be built by CMake
	•	To build it for your Visual Studio Version:
		o	Installing CMake and open it
		o	Defining the source code in “Where is the source code”
		o	Defining the build directory in the “Where to build directory”
		o	Press configure and specify your Visual Studio version
		o	Select X64 in “optional platform for generator”
		o	Press Finish
		o	Press Generate
		o	Now you have the solution in the defined build folder
		o	Open solution and right-click on match_three and set it as startup project	
# Keys
	•	Left-key | A: Slides the falling pieces to the left
	•	Down-key | S: Freefall
	•	Right-key | D: Slides the falling pieces to the right
	•	Space-key: Flips the falling pieces 90-degree clock-wise
# TODO:
	•	Remove game logic bug
	•	Sequential match does not work
	•	Move all game logic to GameLogic script

