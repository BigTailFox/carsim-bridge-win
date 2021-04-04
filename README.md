# carsim-bridge-win
A bridge supporting minimal information exchange between CarSim and other softwares. Can be used on distributed machines.
## quick start
make sure you have installed [lcm](https://github.com/lcm-proj/lcm) first, and
```
mkdir build
cmake ..
make
```
then in your CarSim GUI choose `Models: Self-Contained Solvers`, and make the `External wrapper program` point to the path of `carsim-bridge.exe` which you have built before.
click `Run`, the bridge begins to wait for control and road contact messages, as well as to publish tire-road contact query and vehicle state messages on the lcm channels.
## message defination
there are four messages for a minimal run. You can check them in `msg/simuCarsim.zcm`.
## roadmap
the code in this repo doesn't support test automation yet.
## license
the code of CarSim C API and lcm follow their own license. other code can be used under BSD.
