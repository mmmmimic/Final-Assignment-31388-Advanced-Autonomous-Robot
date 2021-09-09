# Final Assignment - 31388 Advanced Autonomous Robot
## Run the script  
bash main.sh
## Configuration  
- check objConfig.yaml
![avatar](maze.jpg)
## Language
- SMR-CL
- C++
## Process
1. Start from a 60x60 square
2. Go to the first guidemark given in advance
3. Read and Move
4. Is the guidemark number 98?  
- Yes: step 5
- No: step 3
5. Go around the green square
6. Find the object information, including shape, centroid coordinates, orientation, width and height
7. Go back to origin

## Process
- [x] follow guidemark    
- [x] spawn objects
- [x] detect objects
- [x] back to origin
