CC := arm-none-eabi-gcc
SRCS := $(shell find . -name '*.c')
OBJS := $(patsubst %.c,%.o,$(SRCS))

pomodoro: $(OBJS)
	$(CC) -o build/pomodoro $(OBJS)

CFLAGS := \
	--specs=nano.specs \
	-DDEBUG \
	-DSTM32L475xx \
	-DUSE_HAL_DRIVER \
	-fdata-sections \
	-ffunction-sections \
	-g3 \
	-mcpu=cortex-m4 \
	-mfloat-abi=hard \
	-mfpu=fpv4-sp-d16 \
	-mthumb \
	-O0 \
	-std=gnu11 \
	-Wall
#	-fstack-usage \ # temporarily not used

INC := \
	-ICore/Inc \
	-IDrivers/BSP/B-L475E-IOT01 \
	-IDrivers/BSP/Components/Common \
	-IDrivers/BSP/Components/lsm6dsl \
	-IDrivers/CMSIS/Device/ST/STM32L4xx/Include \
	-IDrivers/CMSIS/Include \
	-IDrivers/STM32L4xx_HAL_Driver/Inc \
	-IDrivers/STM32L4xx_HAL_Driver/Inc/Legacy

%.o: %.c
	$(CC) $(INC) $(CFLAGS) -c $< -o build/$@
