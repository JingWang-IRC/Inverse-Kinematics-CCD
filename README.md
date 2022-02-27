# Inverse kinematics with CCD and analytic method
Implement Inverse Kinemarics with Cyclic Coordinate Descent (CCD) and analytic method. 
I only used anamytic method in 2D with only 2 bones, while I used CCD both in 2D and 3D with multiple bones.
In addition, scripted animation using cubic spline is implemented.
OpenGL and C++ are used.
[A short demonstration](https://www.youtube.com/watch?v=mbqRKKrZFag) has been uploaded to Youtube.
For more detailed information, you can refer to the description (PDF) in this repository.
All the code are in **Anime_Assignment2_2D** and **Anime_Assignment2_3D** folders, while the used 3D models are in the **Release** folder.

# Analytic method
When the number of bones are small, for example, only two bones, we can analytically caluculate the rotations of the bones using some simple trigonometry.
[This](https://www.youtube.com/watch?v=RH3iAmMsolo&t=181s) is a wonderful video which explains the analytic method well.
The results will always be correct.
However, this method becomes very hard when to solve IK of lots of bones, for example, 10 bones.
The following 2 images are screenshots of my results of IK in 2D.

<p float="left">
  <img src="https://github.com/xxxiaojing/Inverse-Kinematics-CCD/blob/main/image/2d_1.png" width="400" />
  <img src="https://github.com/xxxiaojing/Inverse-Kinematics-CCD/blob/main/image/2d_2.png" width="400" /> 
</p>

# Cyclic Coordinate Descent (CCD)
CCD is a numerical solution for IK.
From the last bone in the chain of bones, CCD rotate the bones one by one to let them align to the direction to the target.
Usually, in 3D, quaternion is a great tool to rotate the objects.
Without quaternion, calculation of the rotation matrix of the bones will be extremely difficult.
CCD is fast and easy to implement, but it still has problems.
For example, in some situations, even though you only moved the target a bit, the orientation of all the bones may change largely to reach the target point.
The following 2 images are screenshots of my results of IK with CCD in 3D.
Only the left arm of the diamond man is included to compute IK.

<p float="left">
  <img src="https://github.com/xxxiaojing/Inverse-Kinematics-CCD/blob/main/image/3d_1.png" width="400" />
  <img src="https://github.com/xxxiaojing/Inverse-Kinematics-CCD/blob/main/image/3d_2.png" width="400" /> 
</p>

# Cubic spline (interpolation for Key-frames)
To interpolate between Key-frames, linear interpolation is a usual way.
However, linear interpolated motion is not smooth.
Instead of linear interpolation, spline is a good tool for interpolation.
There are lots of splines, in this project, cubic spline is implemented.
[This video](https://www.youtube.com/watch?v=LaolbjAzZvg) explained cubic spline in detail.
In addition, [this paper](http://www.cim.mcgill.ca/~langer/557/10.pdf) significantly helped me implement cubic spline.

# Dependency
The project is build in Visual Studio 2017.
The used library are GLM-0.9.8.5, GLEW-2.1.0, GLFW-3.3.2, GLAD, Assimp5.0.0. 
