C_SRC:= adc.c
CPP_SRC:= test_adc.cpp
BUILD_DIR:=build
OBJ:= $(C_SRC:%.c=$(BUILD_DIR)/%.o) $(CPP_SRC:%.cpp=$(BUILD_DIR)/%.o)
DEP:= $(OBJ:%.o=%.d)
BIN:= $(BUILD_DIR)/test_adc.exe

# to suppress echoing the command invocation
Q:= @
# for output
E:= @echo

# create build dir if it did not exist
$(info $(shell mkdir -p $(BUILD_DIR)))

.PHONY: test
test: $(BIN)
	$(BIN)

$(DEP):

-include $(DEP)

$(BIN): $(OBJ)
	$(E) "LINK $@"
	$(Q) g++ -o $@ $^ -lboost_unit_test_framework-mt

$(CPP_SRC:%.cpp=$(BUILD_DIR)/%.o): $(BUILD_DIR)/%.o: %.cpp $(BUILD_DIR)/%.d
	$(E) "CC   $@"
	$(Q) g++ -c -o $@ $< -MMD -MP -O2 -std=c++17 -Wall -Wextra -DBOOST_TEST_DYN_LINK

$(C_SRC:%.c=$(BUILD_DIR)/%.o): $(BUILD_DIR)/%.o: %.c $(BUILD_DIR)/%.d
	$(E) "C++  $@"
	$(Q) gcc -c -o $@ $< -MMD -MP -O2 -std=c11 -Wall -Wextra

.PHONY:
clean:
	rm -rf $(BUILD_DIR)
