#!/usr/bin/env bash
# Build a talk stage with its presets and collect the outputs used on the slides.
#
#   talk/common/scripts/capture.sh <stage-dir> [preset ...]     (default: every preset of the stage)
#   talk/common/scripts/capture.sh all                          (every stage)
#
# Writes talk/artifacts/<stage>/<preset>/:
#   build.log       configure + build output (error demos are expected to fail; the log is the artifact)
#   memory.txt      --print-memory-usage table
#   size.txt        arm-none-eabi-size -A
#   isr_vector.txt  objdump -s -j .isr_vector
#   reset.txt       disassembly of Reset_Handler (demangled)
#   symbols.txt     nm --size-sort (demangled)
#   firmware.map    linker map
set -u

TALK_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
OBJDUMP=arm-none-eabi-objdump
SIZE=arm-none-eabi-size
NM=arm-none-eabi-nm

presets_of() {
    (cd "$1" && cmake --list-presets 2>/dev/null | sed -n 's/^  "\([^"]*\)".*/\1/p')
}

capture() {
    local stage_dir="$1" preset="$2"
    local stage; stage="$(basename "$stage_dir")"
    local out="$TALK_DIR/artifacts/$stage/$preset"
    local build="$stage_dir/build/$preset"
    mkdir -p "$out"

    { cmake --preset "$preset" -S "$stage_dir" --fresh && cmake --build "$build"; } >"$out/build.log" 2>&1
    local status=$?
    grep -A2 '^Memory region' "$out/build.log" >"$out/memory.txt"

    local elf="$build/firmware.elf"
    if [[ $status -eq 0 && -f "$elf" ]]; then
        "$SIZE" -A "$elf" >"$out/size.txt"
        "$OBJDUMP" -s -j .isr_vector "$elf" >"$out/isr_vector.txt" 2>/dev/null
        "$OBJDUMP" -d -C --disassemble=Reset_Handler "$elf" >"$out/reset.txt"
        "$NM" -C --size-sort -S "$elf" >"$out/symbols.txt"
        cp "$build/firmware.map" "$out/firmware.map" 2>/dev/null
        printf '%-28s %-30s ok\n' "$stage" "$preset"
    else
        rm -f "$out/memory.txt"
        printf '%-28s %-30s build failed (see build.log)\n' "$stage" "$preset"
    fi
}

if [[ $# -lt 1 ]]; then
    sed -n '2,15p' "$0"; exit 1
fi

if [[ "$1" == "all" ]]; then
    for d in "$TALK_DIR"/[0-9]*/; do
        for p in $(presets_of "$d"); do capture "${d%/}" "$p"; done
    done
    exit 0
fi

stage_dir="$(cd "$1" && pwd)"; shift
presets=("$@")
[[ ${#presets[@]} -eq 0 ]] && presets=($(presets_of "$stage_dir"))
for p in "${presets[@]}"; do capture "$stage_dir" "$p"; done
