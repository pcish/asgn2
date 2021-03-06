@types = (
  {'name'=>'Mile', 'parent'=>'Ordinal', 'base'=>'double', 'default'=>'0', 'minimum'=>'0.0',
  'constructor'=>'if (num < minimum) throw Fwk::RangeException("Mile must be a positive number");'},
  {'name'=>'USD', 'parent'=>'Ordinal', 'base'=>'double', 'default'=>'0.0', 'minimum'=>'0.0',
  'constructor'=>'if (num < minimum) throw Fwk::RangeException("USD must be a positive number");'},
  {'name'=>'PackageUnit', 'parent'=>'Ordinal', 'base'=>'int', 'default'=>'0', 'minimum'=>'0',
  'constructor'=>'if (num < minimum) throw Fwk::RangeException("PackageUnit must be a positive number");'},
  {'name'=>'ShipmentCount', 'parent'=>'Ordinal', 'base'=>'unsigned int', 'default'=>'0', 'minimum'=>'0',
  'constructor'=>'if (num < minimum) throw Fwk::RangeException("ShipmentCount must be a positive number");'},
  {'name'=>'SegmentDifficultyUnit', 'parent'=>'Ordinal', 'base'=>'float', 'default'=>'1.0', 'minimum'=>'1.0', 'maximum'=>'5.0',
  'constructor'=>'if (num < minimum || num > maximum) throw Fwk::RangeException("Segment Difficulty must lie between 1.0 and 5.0");'},
  {'name'=>'SegmentCount', 'parent'=>'Ordinal', 'base'=>'unsigned int', 'default'=>'0', 'constructor'=>''},
  {'name'=>'TruckSegmentCount', 'parent'=>'Ordinal', 'base'=>'unsigned int', 'default'=>'0', 'constructor'=>''},
  {'name'=>'BoatSegmentCount', 'parent'=>'Ordinal', 'base'=>'unsigned int', 'default'=>'0', 'constructor'=>''},
  {'name'=>'PlaneSegmentCount', 'parent'=>'Ordinal', 'base'=>'unsigned int', 'default'=>'0', 'constructor'=>''},
  {'name'=>'LocationCount', 'parent'=>'Ordinal', 'base'=>'unsigned int', 'default'=>'0', 'constructor'=>''},
  {'name'=>'PortCount', 'parent'=>'Ordinal', 'base'=>'unsigned int', 'default'=>'0', 'constructor'=>''},
  {'name'=>'TerminalCount', 'parent'=>'Ordinal', 'base'=>'unsigned int', 'default'=>'0', 'constructor'=>''},
  {'name'=>'TruckTerminalCount', 'parent'=>'Ordinal', 'base'=>'unsigned int', 'default'=>'0', 'constructor'=>''},
  {'name'=>'BoatTerminalCount', 'parent'=>'Ordinal', 'base'=>'unsigned int', 'default'=>'0', 'constructor'=>''},
  {'name'=>'PlaneTerminalCount', 'parent'=>'Ordinal', 'base'=>'unsigned int', 'default'=>'0', 'constructor'=>''},
  {'name'=>'Hour', 'parent'=>'Ordinal', 'base'=>'double', 'default'=>'0.0', 'constructor'=>''}
);


open FILE, ">types.h";

print FILE "#ifndef ENGINE_TYPES_H\n";
print FILE "#define ENGINE_TYPES_H\n\n";

print FILE "#include \"fwk/Exception.h\"\n\n";

print FILE "namespace Shipping {\n\n";

foreach $type (@types) {
  print FILE "class $type->{'name'} : public $type->{'parent'}<$type->{'name'}, $type->{'base'}> {\n";
  print FILE "  public:\n";
  if ($type->{'minimum'} ne '') {
    print FILE "    static const $type->{'base'} minimum = $type->{'minimum'};\n"
  }
  if ($type->{'maximum'} ne '') {
      print FILE "    static const $type->{'base'} maximum = $type->{'maximum'};\n"
  }
  print FILE "    $type->{'name'}($type->{'base'} num = $type->{'default'}) : $type->{'parent'}<$type->{'name'}, $type->{'base'}>(num) {\n";
  if ($type->{'constructor'} ne '') {
    print FILE "        $type->{'constructor'}\n";
  }
  print FILE "    }\n";
  print FILE "};\n\n";
}

print FILE "}\n\n";
print FILE "#endif\n";

close FILE;
