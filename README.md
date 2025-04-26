# HandSequenceController for mc_rtc Superbuild

This repository contains a custom controller designed for use with the `mc_rtc` robot control framework, specifically within the `mc_rtc_superbuild` environment. This controller implements a sequence of hand movements for a robot, moving the left hand to a target, returning it to its starting position, moving the right hand to a target, returning it to its starting position, moving both hands to their respective targets, and finally returning both to their starting positions. The sequence then repeats.

## Prerequisites

* **mc_rtc Superbuild:** This controller is intended to be used within a development environment set up using the `mc_rtc_superbuild` repository.

## Installation and Building

Follow these steps to integrate and build this controller within your `mc_rtc_superbuild` environment:

1.  **Clone the `mc_rtc_superbuild` repository:**

    ```bash
    git clone https://github.com/mc-rtc/mc-rtc-superbuild
    cd mc-rtc-superbuild
    ```

2.  **Open the `mc-rtc-superbuild` folder in VSCode:**

    ```bash
    code .
    ```

3.  **Install Recommended Extensions:** VSCode will likely prompt you to install recommended extensions. **Ensure you install the Devcontainer extension.**

4.  **Re-open in Devcontainer:** Once the Devcontainer extension is installed, VSCode will prompt you to re-open the current folder in a devcontainer. **Select the `jammy` image.** This will build the necessary development environment.

5.  **Build `mc_rtc_superbuild`:** Open a new terminal within the VSCode devcontainer and navigate to the root of the `mc-rtc-superbuild` folder. Run the following commands:

    ```bash
    cmake --preset relwithdebinfo
    cmake --build --preset relwithdebinfo
    ```

6.  **Clone this controller repository:** Navigate to the `src` directory within your `mc-rtc-superbuild` clone and clone this `HandSequenceController` repository:

    ```bash
    cd src
    git clone https://github.com/Diegojllo0701/hand_sequence.git
    cd hand_sequence
    ```

7.  **Create a build directory for the controller and configure CMake:**

    ```bash
    mkdir build
    cd build
    cmake ../ -DCMAKE_BUILD_TYPE=RelWithDebInfo
    ```

8.  **Build the controller:**

    ```bash
    make
    ```

9.  **Install the controller:**

    ```bash
    sudo make install
    ```

    This command will install the compiled controller library into the appropriate location within your `mc_rtc` environment, making it available for use.

## Usage

Once the controller is built and installed, you should be able to select and run it by changing the enabled controller in superbuild/install/relwithdebinfo/etc/mc_rtc.yaml to Enabled: [HandSequenceController]
And running the mc_rtc_ticker in rviz with:

```bash
ros2 launch mc_rtc_ticker display.launch
```

## Useful Links
* **mc\_rtc Doxygen Documentation:** [https://jrl.cnrs.fr/mc\_rtc/doxygen.html#](https://jrl.cnrs.fr/mc_rtc/doxygen.html#) - This link provides the complete API documentation for the `mc_rtc` framework.

* **mc\_rtc First Controller Tutorial:** [https://jrl.cnrs.fr/mc\_rtc/tutorials/introduction/first-controller.html](https://jrl.cnrs.fr/mc_rtc/tutorials/introduction/first-controller.html) - This tutorial offers a step-by-step guide to creating your first `mc_rtc` controller, which can be helpful for understanding the basics.

