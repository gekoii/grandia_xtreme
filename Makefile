# Build and verify Grandia Xtreme. Run inside the grandia-build container
# (see tools/docker.sh), after tools/split.sh has generated asm/ and linker_scripts/.

COMPILER ?= ee-gcc2.9-991111
CC_DIR   := tools/compilers/$(COMPILER)

CROSS   := mips-linux-gnu-
AS      := $(CROSS)as
LD      := $(CROSS)ld
OBJCOPY := $(CROSS)objcopy
CC      := $(CC_DIR)/bin/ee-gcc
# Most of the game's C was assembled with the binutils that came with the later
# ee-gcc 2.96 toolchain, not the compiler's own 1999 assembler: only that
# assembler reproduces the original delay-slot filling and leaves out the float
# hazard nops (after mfc1/mtc1, between c.cond.s and bc1t). Some files were built
# differently (another compiler build, or the 1999 assembler); tools/compile_c.py
# holds the default toolchain and the per-file overrides.

ASFLAGS := -EL -march=r5900 -mabi=eabi -G0 -no-pad-sections -I include
CFLAGS  := -O2 -G0 -Iinclude -Isrc
LDFLAGS := -EL --no-check-sections

BUILD := build

# Loadable sections of the main ELF, in rom order
MAIN_SECTIONS := .main .lay_subprog1

all: check

# Keep the generated C assembly around for inspection
.SECONDARY:

# --- assembling / compiling ----------------------------------------------------

# Modern gas aligns every section to 16, but the game's code and data are only
# aligned as far as their addresses need. Every piece is named after where it
# starts (asm pieces by rom offset, C files by address; both bases are 16-aligned),
# so each object's alignment is set to the natural alignment of that address,
# capped at 16. Where the original has zero padding before a piece (e.g. a new
# object file starting 16-aligned), this reproduces it; elsewhere the previous
# piece ends exactly at the start, so no padding is added.
# This applies to .text of code pieces and C files, to .rodata and .data
# pieces (cut around C files' jump tables, constants and data, plan step 6b)
# and to .bss pieces (an overlay's .scommon starts only 8-aligned). Data-only
# C files (src/<binary>/data/) get their .data/.rodata aligned the same way,
# from their markers (tools/data_asm.py; tools/check_data.py checks that gcc's
# own alignment of their objects allows it), and so does the .rodata of a code
# file that claims a range with a `/* rodata: */` marker (a data file before it
# may end with zeros that only this alignment recreates).
# A code piece's empty .bss gets alignment 1, so it doesn't move FIELD's
# 8-aligned .scommon (an overlay's .bss is linked in its loadable section).
TEXT_ALIGN_FIX = $(OBJCOPY) --set-section-alignment .text=$(1) $@
RODATA_ALIGN_FIX = $(OBJCOPY) --set-section-alignment .rodata=$(1) $@
DATA_ALIGN_FIX = $(OBJCOPY) --set-section-alignment .data=$(1) $@
BSS_ALIGN_FIX = $(OBJCOPY) --set-section-alignment .bss=$(1) $@
piece_name = $(basename $(basename $(notdir $(1))))
natural_align = $(if $(filter %0,$(1)),16,$(if $(filter %8,$(1)),8,$(if $(filter %4 %C,$(1)),4,1)))
piece_align = $(call natural_align,$(call piece_name,$(1)))

$(BUILD)/asm/%.o: asm/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<
	$(if $(findstring /data/,$@),,$(call TEXT_ALIGN_FIX,$(call piece_align,$@)) --set-section-alignment .bss=1)
	$(if $(filter %.rodata.o,$@),$(call RODATA_ALIGN_FIX,$(call piece_align,$@)))
	$(if $(filter %.data.o,$@),$(call DATA_ALIGN_FIX,$(call piece_align,$@)))

# An overlay's .bss is part of its file, so splat disassembles it like data
# (ld_bss_is_noload: False) and may write zero floats, which gas refuses in
# .bss: write them as zero words. (Only while a .bss is still assembly; the
# .bss files in src/ (tools/gen_bss.py) replace these pieces.)
$(BUILD)/asm/%.bss.o: asm/%.bss.s
	@mkdir -p $(dir $@)
	sed -E 's/\.float 0$$/.word 0/; s/\.double 0$$/.word 0, 0/' $< > $(@:.o=.s)
	$(AS) $(ASFLAGS) -o $@ $(@:.o=.s)
	$(call BSS_ALIGN_FIX,$(call piece_align,$@))

# C goes through tools/compile_c.py: ee-gcc, then an EE assembler, then modern
# objcopy (the game's C was assembled with the ee-gcc 2.96 toolchain's binutils
# by default). A file can pick another compiler or assembler build with marker
# comments; the assembly is kept next to the object for inspection.
# Any header change rebuilds all C (headers are shared, so this is simpler
# and safer than per-file dependency tracking).
C_HEADERS := $(wildcard include/*.h include/protos/*.h)
# A C file's .rodata gets a global symbol at its start, __rodata_<binary>_<file>:
# names other files use for its strings and constants are defined relative to
# it (tools/c_syms.py), so they move with the file.
RODATA_MARKER = if $(CROSS)readelf -S $@ | grep -q ' \.rodata '; then \
	$(OBJCOPY) --add-symbol __rodata_$(subst /,_,$*)=.rodata:0,global $@; fi
$(BUILD)/src/%.o: src/%.c tools/compile_c.py tools/as_hazard.py $(C_HEADERS)
	@mkdir -p $(dir $@)
	python3 tools/compile_c.py $< $@ -S $(@:.o=.s)
	$(call TEXT_ALIGN_FIX,$(call piece_align,$@))
	$(if $(findstring /data/,$<),python3 tools/data_asm.py align $< $@,@if grep -q '/\* rodata: 0x' $<; then python3 tools/data_asm.py align $< $@; fi)
	@$(RODATA_MARKER)

# --- linking ---------------------------------------------------------------------

-include linker_scripts/SLUS_204.17.d linker_scripts/field.d linker_scripts/battle.d linker_scripts/viewer.d

# The linker scripts: splat's, plus a check that each binary is linked at
# its address in config/memory_map.ld (tools/ld_script.py)
$(BUILD)/ld/SLUS_204.17.ld: linker_scripts/SLUS_204.17.ld tools/ld_script.py
	python3 tools/ld_script.py main $@
$(BUILD)/ld/%.ld: linker_scripts/%.ld tools/ld_script.py
	python3 tools/ld_script.py $* $@

# Names used in C or asm that no object defines (inside another object, a
# string in another C file's .rodata, an alternate name), each relative to
# what contains it, plus every name in the symbol files; see tools/c_syms.py
# and docs/shifting.md. The linker script is a dependency because every split
# rewrites it (asm/ may have changed).
FIXED_LD := config/memory_map.ld config/symbol_exprs.ld
C_SYMS_DEPS := tools/c_syms.py tools/binaries.py tools/data_asm.py config/symbol_addrs.txt $(FIXED_LD)
$(BUILD)/c_syms.ld: $(C_SYMS_DEPS) linker_scripts/SLUS_204.17.ld $(wildcard src/main/??????.c src/main/bss_*.c src/main/data/*.c)
	python3 tools/c_syms.py $@ src/main asm/main --names config/symbol_addrs.txt

# Keep the link inputs in step with link_scripts() in tools/ld_script.py
# (tools/shift_test.py relinks with them and checks it gets the same bytes).
$(BUILD)/SLUS_204.17.elf: $(BUILD)/ld/SLUS_204.17.ld $(BUILD)/c_syms.ld $(FIXED_LD) config/linker_script_extra.ld
	$(LD) $(LDFLAGS) -Map $(@:.elf=.map) -o $@ \
		-T $(BUILD)/ld/SLUS_204.17.ld \
		-T config/memory_map.ld \
		-T config/symbol_exprs.ld \
		-T config/linker_script_extra.ld \
		-T $(BUILD)/c_syms.ld

# Overlays: the same, and every main-executable name they use gets its
# address from the built main executable (they are separate files that hold
# absolute main addresses, so they are relinked against it)
.SECONDEXPANSION:
$(BUILD)/c_syms.%.ld: $(C_SYMS_DEPS) config/symbol_addrs.$$*.txt linker_scripts/$$*.ld $$(wildcard src/$$*/??????.c src/$$*/bss_*.c src/$$*/data/*.c) \
		$(BUILD)/SLUS_204.17.elf linker_scripts/SLUS_204.17.ld
	python3 tools/c_syms.py $@ src/$* asm/$* --names config/symbol_addrs.txt config/symbol_addrs.$*.txt \
		--main-elf $(BUILD)/SLUS_204.17.elf

$(BUILD)/%.elf: $(BUILD)/ld/%.ld $(BUILD)/c_syms.%.ld $(FIXED_LD)
	$(LD) $(LDFLAGS) -Map $(@:.elf=.map) -o $@ \
		-T $(BUILD)/ld/$*.ld \
		-T config/memory_map.ld \
		-T config/symbol_exprs.ld \
		-T $(BUILD)/c_syms.$*.ld

# --- flat images ---------------------------------------------------------------

# Same layout as tools/elf2rom.py produces from the original ELF: each loadable
# section's bytes back to back.
$(BUILD)/SLUS_204.17.rom: $(BUILD)/SLUS_204.17.elf
	@rm -f $@
	@for s in $(MAIN_SECTIONS); do \
		$(OBJCOPY) -O binary -j $$s $< $@.part && cat $@.part >> $@; done
	@rm -f $@.part

$(BUILD)/%.bin: $(BUILD)/%.elf
	$(OBJCOPY) -O binary $< $@

# --- verification ------------------------------------------------------------

check: $(BUILD)/SLUS_204.17.rom $(BUILD)/field.bin $(BUILD)/battle.bin $(BUILD)/viewer.bin
	@tools/check.sh

# --- run-time test images (docs/runtime_test.md) ------------------------------

# Bootable binaries for a shift, checked like tools/shift_test.py:
# build/boot/<name>/ (presets control, shift_field, shift_all_pinned; see
# tools/boot_build.py for custom pads). Run in the container.
boot-%: check
	python3 tools/boot_build.py $*

# A copy of the user's disc image with those binaries, build/iso/<name>.iso
# (tools/iso_patch.py; plain Python, so it can run outside the container,
# where it's faster: `make iso-shift_field`).
iso-%:
	python3 tools/iso_patch.py --dir $(BUILD)/boot/$* --out $(BUILD)/iso/$*.iso

# The whole set for docs/runtime_test.md, from build/boot/{control,shift_field,
# shift_all_pinned} (`tools/docker.sh make boot-control boot-shift_field
# boot-shift_all_pinned` first): each image, a _debug variant (a GRX.INI that
# turns debug mode on) and, for control and shift_all_pinned, a _viewer
# variant (a GRX.INI that boots VIEWER).
TEST_IMAGES := control shift_field shift_all_pinned
iso-images:
	@for n in $(TEST_IMAGES); do \
		python3 tools/iso_patch.py --dir $(BUILD)/boot/$$n --out $(BUILD)/iso/$$n.iso || exit 1; \
		python3 tools/iso_patch.py --no-rar-check --dir $(BUILD)/boot/$$n --grx-ini debug \
			--out $(BUILD)/iso/$${n}_debug.iso || exit 1; \
	done
	@for n in control shift_all_pinned; do \
		python3 tools/iso_patch.py --no-rar-check --dir $(BUILD)/boot/$$n --grx-ini viewer \
			--out $(BUILD)/iso/$${n}_viewer.iso || exit 1; \
	done

# Bisecting a failure of shift_all_pinned (docs/runtime_test.md,
# "Bisecting"): one binary at a time, then one padding point at a time
# (presets in tools/boot_build.py). `tools/docker.sh make boot-bisect`, then
# `make iso-bisect`: each image and its _debug variant; shift_viewer only as
# shift_viewer_viewer.iso (the game never loads VIEWER otherwise).
BISECT_IMAGES := shift_main_pinned shift_main_text shift_main_bss \
	shift_battle_pinned shift_battle_text shift_battle_bss shift_field_all
boot-bisect: $(addprefix boot-,$(BISECT_IMAGES) shift_viewer)
iso-bisect:
	@for n in $(BISECT_IMAGES); do \
		python3 tools/iso_patch.py --dir $(BUILD)/boot/$$n --out $(BUILD)/iso/$$n.iso || exit 1; \
		python3 tools/iso_patch.py --no-rar-check --dir $(BUILD)/boot/$$n --grx-ini debug \
			--out $(BUILD)/iso/$${n}_debug.iso || exit 1; \
	done
	python3 tools/iso_patch.py --no-rar-check --dir $(BUILD)/boot/shift_viewer --grx-ini viewer \
		--out $(BUILD)/iso/shift_viewer_viewer.iso

clean:
	rm -rf $(BUILD)

.PHONY: all check clean iso-images boot-bisect iso-bisect
