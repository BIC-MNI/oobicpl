INCLUDES = $(MNI_INCLUDES)
LDADD = $(MNI_LIBS) -lm

include_HEADERS = mniVolume.h mniLabelVolume.h mniBaseVolume.h
lib_LTLIBRARIES = liboobicpl.la
TESTS = negative_values_test
check_PROGRAMS = $(TESTS)

liboobicpl_la_SOURCES = mniVolume.cc mniLabelVolume.cc

negative_values_test_SOURCES = negativeValuesTest.cc
negative_values_test_LDADD = -L$(libdir) -loobicpl $(MNI_LIBS)




