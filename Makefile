CC := arm-none-eabi-gcc
LINK := arm-none-eabi-gcc
C_SRCS := $(shell find Core Drivers model -name '*.c' -exec basename {} \;)
BIN_DIR := build
PROJECT := pomodoro

INC := \
	-I./Drivers/STM32L4xx_HAL_Driver/Inc \
	-I./Drivers/STM32L4xx_HAL_Driver/Inc/Legacy \
	-I./Core/Inc \
	-I./Drivers/BSP/B-L475E-IOT01 \
	-I./Drivers/BSP/Components/Common \
	-I./Drivers/BSP/Components/lsm6dsl \
	-I./Drivers/CMSIS/Device/ST/STM32L4xx/Include \
	-I./Drivers/CMSIS/Include \

################################################################################
# QP FRAMEWORK

# location of the QP/C framework (if not provided in an env. variable)
ifeq ($(QPC),)
QPC := ../../..
endif

QP_PORT_DIR := $(QPC)/ports/arm-cm/qk/gnu

C_SRCS += \
	qep_hsm.c \
	qep_msm.c \
	qf_act.c \
	qf_actq.c \
	qf_defer.c \
	qf_dyn.c \
	qf_mem.c \
	qf_ps.c \
	qf_qact.c \
	qf_qeq.c \
	qf_qmact.c \
	qf_time.c \
	qk_port.c

# Typically you should not need to change these lines
VPATH    += \
	$(QPC)/src/qf \
	$(QP_PORT_DIR) \
	Core/Src \
	Drivers/BSP/B-L475E-IOT01 \
	Drivers/BSP/Components/lsm6dsl \
	Drivers/STM32L4xx_HAL_Driver/Src \
	model

INC += -I$(QPC)/include -I$(QPC)/src -I$(QP_PORT_DIR)


C_OBJS       := $(patsubst %.c,%.o,$(C_SRCS))
C_OBJS       += startup_stm32l475vgtx.o # Add startup file
TARGET_EXE   := $(BIN_DIR)/$(PROJECT)
C_OBJS_EXT   := $(addprefix $(BIN_DIR)/, $(C_OBJS))
C_DEPS_EXT   := $(patsubst %.o,%.d, $(C_OBJS_EXT))

################################################################################

CFLAGS := \
	--specs=nano.specs \
	-DDEBUG \
	-DSTM32L475xx \
	-DUSE_HAL_DRIVER \
	-O0 \
	-W \
	-Wall \
	-Wextra \
	-Wno-c11-extensions \
	-fdata-sections \
	-ffunction-sections \
	-g3 \
	-mcpu=cortex-m4 \
	-mfloat-abi=hard \
	-mfpu=fpv4-sp-d16 \
	-mthumb \
	-pedantic \
	-std=gnu11 \
	$(INC)

ASMFLAGS := \
	-mcpu=cortex-m4 \
	-g3 \
	-DDEBUG \
	-c \
	-x assembler-with-cpp \
	--specs=nano.specs \
	-mfpu=fpv4-sp-d16 \
	-mfloat-abi=hard \
	-mthumb

LINKFLAGS := \
	-mcpu=cortex-m4 \
	-TSTM32L475VGTX_FLASH.ld \
	--specs=nosys.specs \
	-Wl,-Map="pomodoro-timer.map" \
	-Wl,--gc-sections \
	-static \
	--specs=nano.specs \
	-mfpu=fpv4-sp-d16 \
	-mfloat-abi=hard \
	-mthumb \
	-Wl,--start-group \
	-lc \
	-lm \
	-Wl,--end-group

################################################################################
# RULES

all: $(TARGET_EXE)

$(TARGET_EXE) : $(C_OBJS_EXT)
	$(CC) $(CFLAGS) -c $(QPC)/include/qstamp.c -o $(BIN_DIR)/qstamp.o
	$(LINK) $(LINKFLAGS) $(LIB_DIRS) -o $@ $^ $(BIN_DIR)/qstamp.o


$(BIN_DIR)/startup_stm32l475vgtx.o : Core/Startup/startup_stm32l475vgtx.s
	$(CC) $(ASMFLAGS) -c $< -o $@

$(BIN_DIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean show

clean :
	rm -f $(BIN_DIR)/*.o \
	$(BIN_DIR)/*.d \
	$(TARGET_EXE)

show :
	@echo PROJECT      = $(PROJECT)
	@echo TARGET_EXE   = $(TARGET_EXE)
	@echo VPATH        = $(VPATH)
	@echo C_SRCS       = $(C_SRCS)
	@echo CPP_SRCS     = $(CPP_SRCS)
	@echo C_DEPS_EXT   = $(C_DEPS_EXT)
	@echo C_OBJS_EXT   = $(C_OBJS_EXT)
	@echo C_DEPS_EXT   = $(C_DEPS_EXT)
	@echo CPP_DEPS_EXT = $(CPP_DEPS_EXT)
	@echo CPP_OBJS_EXT = $(CPP_OBJS_EXT)
	@echo LIB_DIRS     = $(LIB_DIRS)
	@echo LIBS         = $(LIBS)
	@echo DEFINES      = $(DEFINES)
