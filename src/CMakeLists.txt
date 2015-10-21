SET(UTLC_SRCS
    main.cc
    Term.cc
)

INCLUDE_DIRECTORIES(../inc)
ADD_EXECUTABLE(utlc ${UTLC_SRCS})
