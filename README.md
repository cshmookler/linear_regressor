# **linear_regressor**

Performs linear regression with the least squares method to find a function of best fit for a set of sample data.

## Build from Source

### 1.&nbsp; Install a C++ compiler, Git, Meson, GNU MP, Eigen3, and [cpp_result](https://github.com/cshmookler/cpp_result).

#### Linux (Arch):

```bash
sudo pacman -S base-devel git meson gmp eigen
```

Follow the instructions [here](https://github.com/cshmookler/cpp_result) to install cpp_result.

### 2.&nbsp; Clone this project.

```
git clone https://github.com/cshmookler/linear_regressor.git
cd linear_regressor
```

### 3.&nbsp; Build this project from source.

```
meson setup build
cd build
ninja
```

### 4.&nbsp; (Optional) Install this project globally.

```
meson install
```

## **TODO**

- [X] Create a structure for representing a linear equation.
- [X] Perform linear regression using the least squares method on any linear equation.
- [ ] Write examples for using the linear regressor.
- [ ] Write tests for all methods.
