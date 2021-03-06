
###### INSTALLATION GUIDE ######
# make external_libs
# make
################################


APT_GET_DEPENDENCIES:= 


REQUIRED_PACKAGES:= 
REQUIRED_LIBS:= -lpthread 


INCLUDE_PATHS:=

SRC_DIR:=./src

EXECUTABLE:=pgcc_indexer







########################## DON'T EDIT BELOW THIS LINE (unless you are a gnu make's expert ##############

SRC := $(shell find $(SRC_DIR) -name '*.cpp') 
OBJS := $(addprefix bin/,$(SRC:.cpp=.o))

all: $(EXECUTABLE) 
$(EXECUTABLE): $(OBJS)

CXXFLAGS := -fPIC -g -Wall -MMD $(addprefix -I,$(INCLUDE_PATHS))
LDFLAGS := -fPIC  
DEPENDS = $(OBJS:.o=.d)    


install:
	@ln -fs `pwd`/pgcc_indexer /usr/local/bin/pgcc_indexer 
	@echo "Installation successful"
	
	
$(EXECUTABLE) : $(OBJS)          
	@echo "Build executable $@"
	@$(CXX) $(OBJS) -o $@ $(LDFLAGS) 
	


bin/%.o: %.cpp
	@mkdir -p `dirname $(@:.o=.d)`
	@touch $(@:.o=.d)
	@echo "Compilation : $< "
	@g++ $(CXXFLAGS) -MMD -c $< -o $@

bin: 
	@mkdir -p bin

clean:
	@rm -f $(EXECUTABLE)
	@rm -rf bin
	


external_libs:
	@echo "We need your password for this : "
	@sudo echo "thank you"
	@sudo apt-get install $(APT_GET_DEPENDENCIES)
	@echo "DONE"
	

-include $(DEPENDS) 
