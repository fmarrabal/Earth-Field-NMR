#Executable related components
EXECUTABLE_TARGET = Tmp/Firmware
EXECUTABLE_SOURCE = $(shell find . -name "*.c" ! -type d -print)
CFLAGS += -D_GNU_SOURCE
CFLAGS += -ggdb -Wall -Og -std=c99 -mcpu=cortex-m4 -mthumb -mthumb-interwork -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -fsingle-precision-constant -fno-exceptions -specs=nosys.specs
LDFLAGS += -g -T CMSIS/stm32f373xb.ld -nostdlib -lc -lgcc -Wl,-Map=$(EXECUTABLE_TARGET:%=%.map)

CFLAGS += -I CMSIS
CC=arm-none-eabi-gcc
EXECUTABLE_OBJS	+= $(EXECUTABLE_SOURCE:%=Tmp/%.o)
DEPENDS         := $(EXECUTABLE_OBJS:%.o=%.d)
#=======================================================================================================================
default: $(EXECUTABLE_TARGET)

clean:
	@rm -rf Tmp

$(EXECUTABLE_TARGET): $(EXECUTABLE_OBJS)
	@echo "Linking target" $(TARGET_EXEC)
	@$(CC) $(CFLAGS) $(EXECUTABLE_OBJS) -o $(EXECUTABLE_TARGET) $(LDFLAGS)
	@arm-none-eabi-size -A $(EXECUTABLE_TARGET)
	@arm-none-eabi-objdump -S -x $(EXECUTABLE_TARGET) > Tmp/Firmware.lst
	@arm-none-eabi-objcopy -O binary $(EXECUTABLE_TARGET) $(EXECUTABLE_TARGET).bin
	@arm-none-eabi-objcopy -O ihex $(EXECUTABLE_TARGET) $(EXECUTABLE_TARGET).hex

-include $(DEPENDS)

Tmp/%.c.o: %.c
	@mkdir -p $(dir $@)
	@echo "Building" $@
	$(CC) $(CFLAGS) -c $< -o $@

Tmp/%.s.o: %.s
	@mkdir -p $(dir $@)
	@echo "Building" $@
	$(CC) $(CFLAGS) -c $< -o $@