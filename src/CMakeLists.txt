INCLUDE_DIRECTORIES(/home/mongodb/mongo/include/mongo /home/mongodb/mongo/include/mongo/bson ~/CRF/nlpbamboo-1.1.2/source/include ~/CRF/nlpbamboo-1.1.2/source/parser
 ~/CRF/nlpbamboo-1.1.2/source/common ~/burst/src/include)

LINK_DIRECTORIES(/usr/local/lib ~/CRF/nlpbamboo-1.1.2/build/source /home/mongodb/twitter/lib)

SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_CFLAGS_DEBUG "-g -o2")

ADD_EXECUTABLE(burst main.cpp seg.cpp cFile.cpp timeUtils.cpp dbHandler.cpp utility.cpp logger.cpp)

TARGET_LINK_LIBRARIES(burst libbamboo.so mongoclient boost_thread boost_filesystem boost_program_options)
