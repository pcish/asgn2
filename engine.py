from cStringIO import StringIO
import ConfigParser

class Attr(object):
    type = None
    name = None
    collection = False
    readonly = False
    def __init__(self, type, name):
        self.type = type
        self.name = name

    def declaration_str(self):
        ret = StringIO()
        if self.collection:
            ret.write('std::vector<%s> %ss_;' % (self.type, self.name))
        else:
            ret.write('%s %s_;' % (self.type, self.name))
        return ret.getvalue()

    def mutator_str(self):
        if self.readonly:
            return ''
        ret = StringIO()
        if self.collection:
            ret.write('void {name}Is(const {type} {name}) {{ {name}s_.push_back({name}); }}'.format(type=self.type, name=self.name));
        else:
            ret.write('void {name}Is(const {type} {name}) {{ {name}_ = {name}; }}'.format(type=self.type, name=self.name));
        return ret.getvalue()

    def accessor_str(self):
        ret = StringIO()
        if self.collection:
            ret.write('{type} {name} const(const unsigned int index) {{ return {name}s_.at(index); }}'.format(type=self.type, name=self.name));
        else:
            ret.write('{type} {name} const() {{ return {name}_; }}'.format(type=self.type, name=self.name));
        return ret.getvalue()

    def isReadonly(self):
        self.readonly = True

    def isCollection(self):
        self.collection = True

class Enum(object):
    name = None
    enumitems = []
    def __init__(self, name):
        self.name = name;

    def append(self, enumitem):
        self.enumitems.append(enumitem)

    def __str__(self, indent=0):
        ret = StringIO()
        ret.write(' ' * indent)
        ret.write('enum %s {\n' % self.name)
        for i in range(len(self.enumitems)):
            ret.write(' ' * indent)
            ret.write('    %s_' % self.enumitems[i])
            if i == len(self.enumitems) - 1:
                ret.write('\n')
            else:
                ret.write(',\n')
        ret.write(' ' * indent)
        ret.write('};\n')
        for i in self.enumitems:
            ret.write(' ' * indent)
            ret.write('static inline {name} {id}() {{ return {id}_; }}\n'.format(name=self.name, id=i))
        ret.write('\n')
        return ret.getvalue()

class Entity(object):
    attrs = []
    enums = []
    classname = None
    parent = None
    def __init__(self, name, parent=None):
        self.attrs = []
        self.enums = []
        self.classname = name
        self.parent = parent

    def attrIs(self, attr):
        self.attrs.append(attr)

    def enumIs(self, enum):
        self.enums.append(enum)

    def parent_ro_attrs(self):
        a = []
        if self.parent is None or self.parent not in classes.keys():
            return a
        a.extend(classes[self.parent].parent_ro_attrs())
        a.extend(classes[self.parent].roattrs())
        return a

    def roattrs(self):
        roattrs = []
        for attr in self.attrs:
            if attr.readonly:
                roattrs.append(attr)
        roattrs.sort()
        return roattrs

    def constructorStr(self):
        ret = StringIO()
        ret.write('%s(' % self.classname)
        roattrs = self.roattrs()
        constructor_args = self.parent_ro_attrs()
        constructor_args.extend(roattrs)
        for i in range(len(constructor_args)):
            ret.write('const %s %s' % (constructor_args[i].type, constructor_args[i].name))
            if i < len(constructor_args) - 1:
                ret.write(', ')
        ret.write(')')
        # initialization list
        if len(roattrs) > 0 or (self.parent and self.parent in classes.keys()):
            ret.write(' : ')
        if self.parent and self.parent in classes.keys():
            ret.write('%s' % classes[self.parent].newCallStr())
            if len(roattrs) > 0:
                ret.write(', ')
        if len(roattrs) > 0:
            for i in range(len(roattrs)):
                ret.write('%s_(%s)' % (roattrs[i].name, roattrs[i].name))
                if i < len(roattrs) - 1:
                    ret.write(', ')
        ret.write(' {}')
        return ret.getvalue()

    def newCallStr(self):
        roattrs = self.roattrs()
        constructor_args = self.parent_ro_attrs()
        constructor_args.extend(roattrs)
        ret = StringIO()
        ret.write('%s(' % self.classname)
        if len(constructor_args) > 0:
            for i in range(len(constructor_args)):
                ret.write('%s' % (constructor_args[i].name,))
                if i < len(constructor_args) - 1:
                    ret.write(', ')
        ret.write(')')
        return ret.getvalue()

    def newInstanceMethodStr(self):
        ret = StringIO()
        roattrs = self.roattrs()
        constructor_args = self.parent_ro_attrs()
        constructor_args.extend(roattrs)
        ret.write('    static Ptr<%s> %sNew(' % (
            self.classname,
            ''.join((self.classname[0].lower(), self.classname[1:]))
        ))
        for i in range(len(constructor_args)):
            ret.write('const %s %s' % (constructor_args[i].type, constructor_args[i].name))
            if i < len(constructor_args) - 1:
                ret.write(', ')
        ret.write(') {\n')
        ret.write('        Ptr<%s> m = new %s;' % (self.classname, self.newCallStr()))
        ret.write('\n        return m;\n    }')
        return ret.getvalue()

    def __str__(self):
        ret = StringIO()
        ret.write('class %s ' % (self.classname,))
        if self.parent is not None:
            ret.write(': public %s ' % (self.parent,))
        ret.write('{\n')
        ret.write('  public:\n')
        ret.write('    ~%s();\n' % self.classname)
        for enum in self.enums:
            ret.write('%s' % enum.__str__(4))
        for attr in self.attrs:
            ret.write('    %s\n' % attr.accessor_str())
            if not attr.readonly:
                ret.write('    %s\n' % attr.mutator_str())
        ret.write('%s' % self.newInstanceMethodStr())
        ret.write('\n  protected:\n')
        ret.write('    %s' % self.constructorStr())
        ret.write('\n  private:\n')
        for attr in self.attrs:
            ret.write('    %s\n' % attr.declaration_str())
        ret.write('};\n')
        return ret.getvalue()

classes = {}
if __name__ == "__main__":
    baseclassOpt = 'BASECLASS'
    enumOpt = 'ENUM'
    config = ConfigParser.ConfigParser()
    config.optionxform = lambda x: x
    config.read('engine.conf')
    sections = config.sections()

    for section in sections:
        if section == 'HEADER' or section == 'FOOTER':
            continue
        if config.has_option(section, baseclassOpt):
            c = Entity(section, config.get(section, baseclassOpt))
        else:
            c = Entity(section)
        for attr, type in config.items(section):
            if attr == baseclassOpt:
                continue
            if attr == enumOpt:
                enumname, enumitems = type.split(' ', 1)
                enumitems = enumitems.split()
                e = Enum(enumname)
                for i in enumitems:
                    e.append(i)
                c.enumIs(e)
                continue
            (type, _, modifiers) = type.partition('$')
            a = Attr(type, attr)
            if modifiers.find('R') >= 0:
                a.isReadonly()
            if modifiers.find('C') >= 0:
                a.isCollection()
            c.attrIs(a)
        classes[section] = c

    print config.get('HEADER', 'content').replace('\\t', '    ').replace('\\n', '')
    for c in classes.values():
        print c
    print config.get('FOOTER', 'content').replace('\\t', '    ').replace('\\n', '')