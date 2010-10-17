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

class Entity(object):
    attrs = []
    classname = None
    parent = None
    def __init__(self, name, parent=None):
        self.attrs = []
        self.classname = name
        self.parent = parent

    def attrIs(self, attr):
        self.attrs.append(attr)

    def __str__(self):
        ret = StringIO()
        ret.write('class %s ' % (self.classname,))
        if self.parent is not None:
            ret.write(': public %s ' % (self.parent,))
        ret.write('{\n')
        ret.write('  public:\n')
        for attr in self.attrs:
            ret.write('    %s\n' % attr.accessor_str())
            if not attr.readonly:
                ret.write('    %s\n' % attr.mutator_str())
        ret.write('\n  private:\n')
        for attr in self.attrs:
            ret.write('    %s\n' % attr.declaration_str())
        ret.write('};\n')
        return ret.getvalue()

if __name__ == "__main__":
    baseclassOpt = 'BASECLASS'
    config = ConfigParser.ConfigParser()
    config.optionxform = str
    config.read('engine.conf')
    sections = config.sections()
    for section in sections:
        if config.has_option(section, baseclassOpt):
            c = Entity(section, config.get(section, baseclassOpt))
        else:
            c = Entity(section)
        for attr, type in config.items(section):
            if attr == baseclassOpt:
                continue
            (type, _, modifiers) = type.partition('$')
            a = Attr(type, attr)
            if modifiers.find('R') >= 0:
                a.isReadonly()
            if modifiers.find('C') >= 0:
                a.isCollection()
            c.attrIs(a)
        print c