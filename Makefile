CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2
INCLUDES = -Iinclude
LDFLAGS = -pthread 

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

ASCII85_SRC = $(SRC_DIR)/ascii85.cpp
MAIN_SRC = $(SRC_DIR)/main.cpp
 
ASCII85_OBJ = $(OBJ_DIR)/ascii85.o
MAIN_OBJ = $(OBJ_DIR)/main.o
 
ASCII85_EXE = $(BUILD_DIR)/ascii85
GTEST_EXE = $(BUILD_DIR)/ascii85_tests

all: $(ASCII85_EXE) $(GTEST_EXE)

$(ASCII85_EXE): $(MAIN_OBJ) $(ASCII85_OBJ) | $(OBJ_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^

$(GTEST_EXE): $(OBJ_DIR)/test_ascii85.o $(ASCII85_OBJ) | $(OBJ_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^ -lgtest -lgtest_main

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: tests/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) tests/__pycache__

test: $(ASCII85_EXE) $(GTEST_EXE)
	@echo "=== Запуск тестов ==="
	@echo "GoogleTest:"
	@$(GTEST_EXE)
	@echo ""
	@echo "Python тесты на случайных данных:"
	@python3 tests/test_random_data.py

test-python:
	@python3 -m unittest -v tests/test_random_data.py

test-gtest: $(GTEST_EXE)
	@echo "=== Запуск GoogleTest ==="
	@$(GTEST_EXE)

help:
	@echo "Доступные цели:"
	@echo "  all             - Собрать основную программу и unit-тесты"
	@echo "  clean           - Удалить все собранные файлы"
	@echo "  test            - Запустить Google и Python тесты"
	@echo "  test-python     - Запустить Python тесты"
	@echo "  test-gtest      - Запустить GoogleTest"
	@echo "  run ARGS='...'  - Запустить ascii85 с аргументами (напр., ARGS='-db')"
	@echo "  docs-create     - Сгенерировать документацию Doxygen в docs/html"
	@echo "  docs-open       - Открыть документацию в браузере"
	
.PHONY: all clean test help run docs-create docs-open

$(MAIN_OBJ): $(INCLUDE_DIR)/ascii85.h

run: $(ASCII85_EXE)
	@echo "=== Запуск ascii85 ==="
	@echo "Завершить ввод: Ctrl-D"
	@$(ASCII85_EXE) $(ARGS)

docs-create:
	doxygen Doxyfile
	@echo "Документация создана в docs/html"

docs-open:
	xdg-open docs/html/index.html