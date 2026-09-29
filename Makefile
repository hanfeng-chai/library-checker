# make [-jN] [-k]; <problem> is the directory name, without its category.
#   build[-<problem>[-<solution>]]          compile submissions/ and src/
#   gen[-<problem>] [GEN_ARGS=--clean]     generate (or clean) official data
#   check[-<problem>[-<solution>[-<case>]]] check existing data; cache each AC
#   bench[-<problem>[-<solution>]]         cache perf stat reports for all cases
# Run gen before check/bench. Case names are input stems, e.g. example_00.

.DEFAULT_GOAL := build
.DELETE_ON_ERROR:

# Avoid competing measurements, including when invoked with -j.
ifneq ($(filter bench bench-% bench/%,$(MAKECMDGOALS)),)
.NOTPARALLEL:
endif

CXX ?= g++
RUSTC ?= rustc
FLG := $(patsubst -I%,-I$(CURDIR)/%,$(file <cxx_flags.txt))
RFL := $(strip $(file <rust_flags.txt))
$(if $(FLG),,$(error cxx_flags.txt is missing or empty))
$(if $(RFL),,$(error rust_flags.txt is missing or empty))

CPP := $(wildcard submissions/*/*/*.cpp src/*/*/*.cpp)
RS  := $(wildcard submissions/*/*/*.rs src/*/*/*.rs)
BIN := $(patsubst src/%,build/%,$(patsubst submissions/%,build/%,$(basename $(CPP) $(RS))))
PROB := $(patsubst problems/%/info.toml,%,$(filter-out problems/test/%,$(wildcard problems/*/*/info.toml)))
GEN := $(addprefix gen-,$(notdir $(PROB)))
SOL = $(notdir $(filter build/$(1)/%,$(BIN)))
CASES = $(basename $(notdir $(wildcard problems/$(1)/in/*.in)))
AC = $(addprefix check/$(1)/$(2)/,$(addsuffix .ac,$(call CASES,$(1))))

.PHONY: build gen check bench $(GEN)
build: $(BIN)
gen: $(GEN)
check: $(addprefix check-,$(notdir $(PROB)))
bench: $(addprefix bench-,$(notdir $(PROB)))

# Compile from the source directory for submissions with relative file access.
# Absolute source/include paths keep the compiler's .d prerequisites valid here.
DEP = -MMD -MP -MF $(abspath $@).d -MT $@
GXX = @mkdir -p $(@D); cd $(dir $<) && \
      { $(CXX) $(abspath $<) -o $(abspath $@) $(FLG) $(DEP) 2>/dev/null \
        || $(CXX) $(abspath $<) -o $(abspath $@) $(FLG) $(DEP) -fexceptions; }
RST = @mkdir -p $(@D); cd $(dir $<) && \
      { $(RUSTC) --edition 2024 $(notdir $<) -o $(abspath $@) $(RFL) 2>/dev/null \
        || $(RUSTC) --edition 2021 $(notdir $<) -o $(abspath $@) $(RFL); }

vpath %.cpp submissions src
vpath %.rs submissions src
build/%: %.cpp cxx_flags.txt
	$(GXX)
build/%: %.rs rust_flags.txt
	$(RST)

-include $(wildcard $(addsuffix .d,$(BIN)))

$(GEN): gen-%:
	cd problems && CXXFLAGS="$(FLG) -fexceptions" python3 generate.py $(GEN_ARGS) $(filter %/$*,$(PROB))/info.toml

CHK_SOL = $(patsubst %/,%,$(dir $*))
CHK_BIN = build/$(CHK_SOL)
CHK_PROB = problems/$(patsubst %/,%,$(dir $(CHK_SOL)))
CHK_CASE = $(notdir $*)
# tmpfs preserves the mmap EOF padding expected by some public submissions.
CHK_STAGE ?= /dev/shm
.SECONDEXPANSION:

check/%.ac: $$(CHK_BIN) $$(CHK_PROB)/in/$$(CHK_CASE).in $$(CHK_PROB)/out/$$(CHK_CASE).out $$(CHK_PROB)/checker Makefile
	@set -eu; mkdir -p $(@D); rm -f $@; \
	t=$$(mktemp -d $(CHK_STAGE)/lc-XXXXXX); trap 'rm -rf "$$t"' EXIT; \
	cp $(word 2,$^) "$$t/in"; \
	$(CHK_BIN) < "$$t/in" > "$$t/out" 2> "$$t/err" \
	  || { r=$$?; echo "$(CHK_SOL) exit $$r on $(CHK_CASE): $$(head -c 200 "$$t/err")"; exit 1; }; \
	$(CHK_PROB)/checker "$$t/in" "$$t/out" $(word 3,$^) > "$$t/log" 2>&1 \
	  || { echo "$(CHK_SOL) wrong on $(CHK_CASE): $$(head -c 200 "$$t/log")"; exit 1; }; \
	touch $@

# Directory prerequisites also invalidate reports when cases are added/removed.
BENCH_PROB = problems/$(dir $*)
bench/%.txt: build/% tools/bench.py Makefile \
  $$(addprefix $$(BENCH_PROB),checker in out) \
  $$(wildcard $$(BENCH_PROB)in/*.in $$(BENCH_PROB)out/*.out)
	@set -eu; mkdir -p $(@D); rm -f $@; \
	t=$$(mktemp $@.XXXXXX); trap 'rm -f "$$t"' EXIT; \
	python3 tools/bench.py $< $(BENCH_PROB) > "$$t"; mv "$$t" $@

define PROBLEM_RULES
.PHONY: build-$(notdir $(1)) check-$(notdir $(1)) bench-$(notdir $(1))
build-$(notdir $(1)): $(filter build/$(1)/%,$(BIN))
check-$(notdir $(1)): $(foreach s,$(call SOL,$(1)),check-$(notdir $(1))-$(s))
bench-$(notdir $(1)): $(foreach s,$(call SOL,$(1)),bench-$(notdir $(1))-$(s))
endef

define SOLUTION_RULES
.PHONY: build-$(notdir $(1))-$(2) check-$(notdir $(1))-$(2) bench-$(notdir $(1))-$(2)
build-$(notdir $(1))-$(2): build/$(1)/$(2)
check-$(notdir $(1))-$(2): $(call AC,$(1),$(2))
	@test -n "$(call CASES,$(1))" || { echo 'Run make gen-$(notdir $(1)) first'; exit 1; }
check-$(notdir $(1))-$(2)-%: check/$(1)/$(2)/%.ac ; @:
bench-$(notdir $(1))-$(2): bench/$(1)/$(2).txt
endef

$(foreach p,$(PROB),$(eval $(call PROBLEM_RULES,$(p)))\
  $(foreach s,$(call SOL,$(p)),$(eval $(call SOLUTION_RULES,$(p),$(s)))))
