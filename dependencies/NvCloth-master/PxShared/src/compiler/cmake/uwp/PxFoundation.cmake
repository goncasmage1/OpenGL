#
# Build PxFoundation
#

# Can no longer just use LIBPATH_SUFFIX since it depends on build type
IF(CMAKE_CL_64)
	SET(RESOURCE_LIBPATH_SUFFIX "x64")
ELSEIF(${PX_OUTPUT_ARCH} STREQUAL "arm")
	SET(RESOURCE_LIBPATH_SUFFIX "arm")
ELSE(CMAKE_CL_64)
	SET(RESOURCE_LIBPATH_SUFFIX "x86")
ENDIF(CMAKE_CL_64)

SET(PXFOUNDATION_LIBTYPE SHARED)

SET(PXFOUNDATION_RESOURCE_FILE
	${PXSHARED_SOURCE_DIR}/compiler/resource_${RESOURCE_LIBPATH_SUFFIX}/PxFoundation.rc
)
SOURCE_GROUP(resource FILES ${PXFOUNDATION_RESOURCE_FILE})

SET(PXFOUNDATION_PLATFORM_HEADERS
	${PXSHARED_ROOT_DIR}/include/foundation/windows/PxWindowsIntrinsics.h	
)
SOURCE_GROUP("include\\windows" FILES ${PXFOUNDATION_PLATFORM_HEADERS})

SET(PXFOUNDATION_PLATFORM_SOURCE
	${LL_SOURCE_DIR}/src/windows/PsWindowsAtomic.cpp
	${LL_SOURCE_DIR}/src/windows/PsWindowsCpu.cpp
	${LL_SOURCE_DIR}/src/windows/PsWindowsFPU.cpp
	${LL_SOURCE_DIR}/src/windows/PsWindowsMutex.cpp
	${LL_SOURCE_DIR}/src/windows/PsWindowsPrintString.cpp
	${LL_SOURCE_DIR}/src/windows/PsWindowsSList.cpp
	${LL_SOURCE_DIR}/src/windows/PsWindowsSocket.cpp
	${LL_SOURCE_DIR}/src/windows/PsWindowsSync.cpp
	${LL_SOURCE_DIR}/src/windows/PsWindowsTime.cpp
)
SOURCE_GROUP("src\\src\\windows" FILES ${PXFOUNDATION_PLATFORM_SOURCE})

SET(PXFOUNDATION_UWP_PLATFORM_SOURCE
	${LL_SOURCE_DIR}/src/windows/PsUWPThread.cpp
)
SOURCE_GROUP("src\\src\\uwp" FILES ${PXFOUNDATION_UWP_PLATFORM_SOURCE})


SET(PXFOUNDATION_PLATFORM_SOURCE_HEADERS
	${LL_SOURCE_DIR}/include/windows/PsWindowsAoS.h
	${LL_SOURCE_DIR}/include/windows/PsWindowsFPU.h
	${LL_SOURCE_DIR}/include/windows/PsWindowsInclude.h
	${LL_SOURCE_DIR}/include/windows/PsWindowsInlineAoS.h
	${LL_SOURCE_DIR}/include/windows/PsWindowsIntrinsics.h
	${LL_SOURCE_DIR}/include/windows/PsWindowsLoadLibrary.h
	${LL_SOURCE_DIR}/include/windows/PsWindowsTrigConstants.h
)
SOURCE_GROUP("src\\include\\windows" FILES ${PXFOUNDATION_PLATFORM_SOURCE_HEADERS})

SET(PXFOUNDATION_PLATFORM_SOURCE_HEADERS_2	
	${LL_SOURCE_DIR}/include/unix/PsUnixInlineAoS.h	
	${LL_SOURCE_DIR}/include/unix/PsUnixTrigConstants.h
)
SOURCE_GROUP("src\\include\\unix" FILES ${PXFOUNDATION_PLATFORM_SOURCE_HEADERS_2})

SET(PXFOUNDATION_PLATFORM_SOURCE_HEADERS_3
	${LL_SOURCE_DIR}/include/unix/neon/PsUnixNeonAoS.h
	${LL_SOURCE_DIR}/include/unix/neon/PsUnixNeonInlineAoS.h
)
SOURCE_GROUP("src\\include\\unix\\neon" FILES ${PXFOUNDATION_PLATFORM_SOURCE_HEADERS_3})

INSTALL(FILES ${PXFOUNDATION_PLATFORM_HEADERS} DESTINATION include/foundation/windows)

INSTALL(FILES ${PXFOUNDATION_PLATFORM_SOURCE_HEADERS} DESTINATION src/foundation/include/windows)
INSTALL(FILES ${PXFOUNDATION_PLATFORM_SOURCE_HEADERS_2} DESTINATION src/foundation/include/unix)
INSTALL(FILES ${PXFOUNDATION_PLATFORM_SOURCE_HEADERS_3} DESTINATION src/foundation/include/unix/neon)

SET(PXFOUNDATION_PLATFORM_FILES
	${PXFOUNDATION_PLATFORM_HEADERS}
	${PXFOUNDATION_PLATFORM_SOURCE}
	${PXFOUNDATION_UWP_PLATFORM_SOURCE}
	${PXFOUNDATION_PLATFORM_SOURCE_HEADERS}
	${PXFOUNDATION_RESOURCE_FILE}
)

SET(PXFOUNDATION_PLATFORM_INCLUDES
	${LL_SOURCE_DIR}/include/windows
)

SET(PXFOUNDATION_COMPILE_DEFS
	# Common to all configurations
	${PXSHARED_UWP_COMPILE_DEFS};PX_FOUNDATION_DLL=1;

	$<$<CONFIG:debug>:${PXSHARED_UWP_DEBUG_COMPILE_DEFS};>
	$<$<CONFIG:checked>:${PXSHARED_UWP_CHECKED_COMPILE_DEFS};>
	$<$<CONFIG:profile>:${PXSHARED_UWP_PROFILE_COMPILE_DEFS};>
	$<$<CONFIG:release>:${PXSHARED_UWP_RELEASE_COMPILE_DEFS};>
)

SET(PXFOUNDATION_PLATFORM_INTERFACE_HEADERS	"$<INSTALL_INTERFACE:include/foundation/windows>$<BUILD_INTERFACE:${PXSHARED_ROOT_DIR}/include/foundation/windows>")

