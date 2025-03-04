cd /Users/liu.amy05/Documents/mini-maya/assignment_package/build

cmake -DUSD_ROOT="/Users/liu.amy05/usd" -DCMAKE_INSTALL_PREFIX="/Users/liu.amy05/Documents/mini-maya/assignment_package" ..

cmake --build  . -j8 -- my-mini-maya

