# usd_library(my-mini-maya
    
#     PUBLIC_HEADERS_INSTALL_PREFIX
#             ${ORGANIZATION}
    
#     PYTHON_INSTALL_PREFIX
#         ${ORGANIZATION}
        
#     PUBLIC_CLASSES
#         mygl
        
#     PUBLIC_HEADERS
#         display.h
#         la.h
#         mainwindow.h
#         mesh.h
#         mygl.h
#         scene/face.h
#         scene/halfedge.h
#         scene/joint.h
#         scene/skeleton.h
#         scene/vertex.h
#         shaderprogram.h
#         utils.h
#         drawable.h
#         camera.h
#         cameracontrolshelp.h
#         openglcontext.h
#         scene/squareplane.h
#         smartpointerhelp.h

#     CPPFILES
#         main.cpp
#         display.cpp
#         main.cpp
#         mainwindow.cpp
#         mesh.cpp
#         mygl.cpp
#         scene/face.cpp
#         scene/halfedge.cpp
#         scene/joint.cpp
#         scene/skeleton.cpp
#         scene/vertex.cpp
#         shaderprogram.cpp
#         utils.cpp
#         la.cpp
#         drawable.cpp
#         camera.cpp
#         cameracontrolshelp.cpp
#         openglcontext.cpp
#         scene/squareplane.cpp

#     LIBRARIES
#         usd
# )

usd_executable(my-mini-maya
    CPPFILES
        main.cpp
        display.cpp
        main.cpp
        mainwindow.cpp
        mesh.cpp
        mygl.cpp
        scene/face.cpp
        scene/halfedge.cpp
        scene/joint.cpp
        scene/skeleton.cpp
        scene/vertex.cpp
        shaderprogram.cpp
        utils.cpp
        la.cpp
        drawable.cpp
        camera.cpp
        cameracontrolshelp.cpp
        openglcontext.cpp
        scene/squareplane.cpp
        display.h
        la.h
        mainwindow.h
        mesh.h
        mygl.h
        scene/face.h
        scene/halfedge.h
        scene/joint.h
        scene/skeleton.h
        scene/vertex.h
        shaderprogram.h
        utils.h
        drawable.h
        camera.h
        cameracontrolshelp.h
        openglcontext.h
        scene/squareplane.h
        smartpointerhelp.h
        mainwindow.ui
        cameracontrolshelp.ui
        mycpp/glmutils.h
        mycpp/MyCpp_global.h
        mycpp/mydefines.h
        mycpp/myutils.cpp
        mycpp/myutils.h

    LIBRARIES
        usd
)