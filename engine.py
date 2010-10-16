from cStringIO import StringIO
import ConfigParser

class Attr(object):
    type = None
    name = None
    collection = None
    def __init__(self, type, name, collection=None):
        self.type = type
        self.name = name
        self.collection = collection

    def declaration_str(self):
        ret = StringIO()
        ret.write('%s %s_;' % (self.type, self.name))
        return ret.getvalue()

    def mutator_str(self):
        ret = StringIO()
        ret.write('void {name}Is(const {type} {name}) {{ {name}_ = {name}; }}'.format(type=self.type, name=self.name));
        return ret.getvalue()

    def accessor_str(self):
        ret = StringIO()
        ret.write('{type} {name} const() {{ return {name}_; }}'.format(type=self.type, name=self.name));
        return ret.getvalue()

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
            ret.write('    %s\n' % attr.mutator_str())
        ret.write('  private:\n')
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
            c.attrIs(Attr(type, attr))
        print c