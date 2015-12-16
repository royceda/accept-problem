#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_DLIB_EXT=dylib
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Parser.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../LIB/cplex/lib/x86-64_osx/static_pic -L../../LIB/concert/lib/x86-64_osx/static_pic ../../LIB/cplex/lib/x86-64_osx/static_pic/libilocplex.a -lpthread ../../LIB/concert/lib/x86-64_osx/static_pic/libconcert.a ../../LIB/cplex/lib/x86-64_osx/static_pic/libcplex.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sddp

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sddp: ../../LIB/cplex/lib/x86-64_osx/static_pic/libilocplex.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sddp: ../../LIB/concert/lib/x86-64_osx/static_pic/libconcert.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sddp: ../../LIB/cplex/lib/x86-64_osx/static_pic/libcplex.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sddp: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sddp ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Parser.o: Parser.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DIL_STD -I../../LIB/cplex/include -I../../LIB/concert/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Parser.o Parser.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DIL_STD -I../../LIB/cplex/include -I../../LIB/concert/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sddp

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
