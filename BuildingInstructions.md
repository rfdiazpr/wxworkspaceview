# Building Instructions #

wxWorkspaceView ships with a solution file that can be built using Microsoft Visual Studio 2005. On Windows. Other compilers and platforms are currently unsupported.

# Requirements #

The wxWorkspaceView library depends on [wxWidgets 2.8](http://www.wxwidgets.org). But it might also build with wxWidgets 2.6, although that's completely unsupported.

The project file is configurated to look for wxWidgets in "../wxWidgets-2.8.0". So you either have to place wxWidgets there, or add wxWidgets' header and lib paths to your Visual Studio's search paths.