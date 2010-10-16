@types = (
  ['Mile', 'Ordinal', 'unsigned int', ''],
  ['USD', 'Ordinal', 'double', ''],
  ['PackageUnit', 'Ordinal', 'unsigned int', ''],
  ['SegmentDifficultyUnit', 'Ordinal', 'float', 'if (num < 1.0 || num > 5.0) throw ValueError("Segment Difficulty must lie between 1.0 and 5.0");']
);


open FILE, ">types.h";

print FILE "#ifndef ENGINE_TYPES_H\n";
print FILE "#define ENGINE_TYPES_H\n\n";

print FILE "#include \"Engine.h\"\n\n";

print FILE "namespace Shipping {\n\n";

foreach $type (@types) {
  print FILE "class @{$type}[0] : public @{$type}[1]<@{$type}[0], @{$type}[2]> {\n";
  print FILE "  public:\n";
  print FILE "    @{$type}[0](@{$type}[2] num) : @{$type}[1]<@{$type}[0], @{$type}[2]>(num) {\n";
  if (@{$type}[3] ne '') {
    print FILE "        @{$type}[3]\n";
  }
  print FILE "    }\n";
  print FILE "};\n\n";
}

print FILE "}\n\n";
print FILE "#endif\n";

close FILE;
