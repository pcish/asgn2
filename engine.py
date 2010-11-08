from cStringIO import StringIO
import ConfigParser

class IndentedString(object):
    string = None
    indent = 0
    def __init__(self, indent=0):
        self.string = StringIO()
        self.indent = indent

    def write(self, string):
        self.string.write(' ' * self.indent)
        self.string.write(string)

    def getvalue(self):
        return self.string.getvalue()

    def set_indent(self, new_indent):
        self.indent = new_indent

class Attr(object):
    type = None
    name = None
    collection = False
    readonly = False
    virtual = False
    complex = False
    def __init__(self, type, name):
        self.type = type
        self.name = name

    def declaration_str(self):
        ret = StringIO()
        if self.collection:
            ret.write('std::vector<%s > %ss_;' % (self.type, self.name))
        else:
            ret.write('%s %s_;' % (self.type, self.name))
        return ret.getvalue()

    def mutator_str(self):
        if self.readonly:
            return ''
        ret = StringIO()
        if self.virtual:
            ret.write('virtual ')
        if self.type.endswith('*'):
            ret.write('void {name}Is({type} {name})'.format(type=self.type, name=self.name))
        else:
            ret.write('void {name}Is(const {type} {name})'.format(type=self.type, name=self.name))
        if self.complex:
            ret.write(';')
        elif self.collection:
            ret.write(' {{ {name}s_.push_back({name}); }}'.format(type=self.type, name=self.name))
        else:
            ret.write(' {{ if ({name}_ == {name}) return; {name}_ = {name}; if (notifiee_) notifiee_->on{capitalname}(); }}'.format(type=self.type, name=self.name, capitalname=''.join((self.name[0].upper(), self.name[1:]))))
        if self.collection:
            ret.write('\n    ')
            if self.virtual:
                ret.write('virtual ')
            ret.write('void {name}Is(const unsigned int index, {type} {name})'.format(type=self.type, name=self.name))
            if self.complex:
                ret.write(';')
            else:
                ret.write(' {{ {name}s_[index] = {name}; }}'.format(type=self.type, name=self.name))
        return ret.getvalue()

    def accessor_str(self):
        ret = StringIO()
        if self.virtual:
            ret.write('virtual ')
        if self.collection:
            ret.write('{type} {name}(const unsigned int index) const'.format(type=self.type, name=self.name))
        else:
            ret.write('{type} {name}() const'.format(type=self.type, name=self.name))

        if self.complex:
            ret.write(';')
        elif self.collection:
            ret.write(' {{ return {name}s_.at(index); }}'.format(type=self.type, name=self.name))
        else:
            ret.write(' {{ return {name}_; }}'.format(type=self.type, name=self.name))
        return ret.getvalue()

    def isReadonly(self):
        self.readonly = True

    def isCollection(self):
        self.collection = True

    def isVirtual(self):
        self.virtual = True

    def isComplex(self):
        self.complex = True

class Enum(object):
    name = None
    enumitems = []
    def __init__(self, name):
        self.name = name;
        self.enumitems = []

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
    constructorContents = ''
    def __init__(self, name, parent=None):
        self.attrs = []
        self.enums = []
        self.classname = name
        self.parent = parent
        self.constructorContents = ''

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
        ret = IndentedString()
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
        ret.write(' {')
        if len(self.constructorContents) > 0:
            ret.write('\n')
            ret.set_indent(8)
            ret.write(self.constructorContents)
            ret.write('\n')
            ret.set_indent(4)
        ret.write('}\n')
        #ret.write(' {\n')
        #ret.write('        if (notifiee_) notifiee_->on%s();\n' % self.classname)
        #ret.write('    }\n')
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

    def newInstanceMethodStr(self, base_indent=0, modifier='', prefix='', body=True):
        ret = IndentedString(base_indent)
        roattrs = self.roattrs()
        constructor_args = self.parent_ro_attrs()
        constructor_args.extend(roattrs)
        ret.write('%s Ptr<%s> %s%sNew(' % (
            modifier, self.classname, prefix,
            ''.join((self.classname[0].lower(), self.classname[1:]))
        ))
        ret.set_indent(0)
        for i in range(len(constructor_args)):
            typename = constructor_args[i].type
            for enum in self.enums:
                if constructor_args[i].type == enum.name:
                    typename = '::'.join((self.classname, constructor_args[i].type))
            ret.write('const %s %s' % (typename, constructor_args[i].name))
            if i < len(constructor_args) - 1:
                ret.write(', ')
        ret.write(')')
        if not body:
            ret.write(';\n')
            return ret.getvalue()
        ret.write('{\n')
        ret.set_indent(base_indent + 4)
        ret.write('Ptr<%s> m = new %s;\n' % (self.classname, self.newCallStr()))
        ret.write('return m;\n')
        ret.set_indent(base_indent)
        ret.write('}\n')
        return ret.getvalue()

    def notifiee_str(self):
        ret = IndentedString(4)
        ret.write('class Notifiee : public virtual Fwk::NamedInterface::Notifiee {\n')
        ret.write('  public:\n')
        ret.set_indent(8)
        ret.write('virtual void notifierIs(Fwk::Ptr<%s> notifier) {\n' % self.classname)
        ret.set_indent(12)
        ret.write('if (notifier_ == notifier) return;\n')
        ret.write('if (notifier_) notifier->notifieeIs(0);\n')
        ret.write('notifier_ = notifier;\n')
        ret.write('notifier_->referencesDec();\n')
        ret.write('notifier_->notifieeIs(this);\n')
        ret.set_indent(8)
        ret.write('}\n')
        ret.write('static Fwk::Ptr<%s::Notifiee> notifieeNew() {\n' % self.classname)
        ret.set_indent(12)
        ret.write('Fwk::Ptr<%s::Notifiee> n = new Notifiee();\n' % self.classname)
        ret.write('return n;\n')
        ret.set_indent(8)
        ret.write('}\n')
        #ret.write('virtual void on%s() {}\n' % self.classname)
        for attr in self.attrs:
            if attr.readonly:
                continue
            ret.write('virtual void on%s() {}\n' % ''.join((attr.name[0].upper(), attr.name[1:])))
        ret.write('virtual void onDel(%s *p) {}\n' % self.classname)
        ret.set_indent(4)
        ret.write('  protected:\n')
        ret.set_indent(8)
        ret.write('Fwk::Ptr<%s> notifier_;\n' % self.classname)
        ret.write('Notifiee() : notifier_(0) {}\n')
        ret.set_indent(4)
        ret.write('};\n')
        ret.write('Ptr<%s::Notifiee> notifiee() const { return notifiee_; }\n' % self.classname)
        return ret.getvalue()

    def notifiee_protected_str(self):
        ret = IndentedString(4)
        ret.write('Ptr<%s::Notifiee> notifiee_;\n' % self.classname)
        ret.write('void notifieeIs(%s::Notifiee* n) const {\n' % self.classname)
        ret.write('    {name}* me = const_cast<{name}*>(this);\n'.format(name=self.classname))
        ret.write('    me->notifiee_ = n;\n')
        ret.write('}\n')
        return ret.getvalue()

    def __str__(self):
        ret = StringIO()
        ret.write('class %s ' % (self.classname,))
        if self.parent is not None:
            ret.write(': public %s ' % (self.parent,))
        ret.write('{\n')
        #ret.write('    %s' % self.newInstanceMethodStr(0, 'friend', 'EngineManager::', False))
        ret.write('    friend class EngineManager;\n')
        ret.write('  public:\n')
        ret.write('    ~%s() { if (notifiee_) notifiee_->onDel(this); }\n' % self.classname)
        #ret.write('    ShippingNetwork* shippingNetwork() { return network_; }\n')
        for enum in self.enums:
            ret.write('%s' % enum.__str__(4))
        for attr in self.attrs:
            ret.write('    %s\n' % attr.accessor_str())
            if not attr.readonly:
                ret.write('    %s\n' % attr.mutator_str())
        #ret.write('%s' % self.newInstanceMethodStr(4))
        ret.write(self.notifiee_str())
        ret.write('  protected:\n')
        ret.write('    %s' % self.constructorStr())
        ret.write(self.notifiee_protected_str())
        ret.write('\n  private:\n')
        for attr in self.attrs:
            ret.write('    %s\n' % attr.declaration_str())
        ret.write('    {name}(const {name}& o);\n'.format(name=self.classname))
        #ret.write('    ShippingNetwork *network_;\n')
        ret.write('};\n')
        return ret.getvalue()

class ManagerClass(Entity):
    def __init__(self):
        self.classname = 'EngineManager'
        for c in classes.values():
            self.attrs.append(Attr('', '%sNew' % (''.join((c.classname[0].lower(), c.classname[1:])))))

    def __str__(self):
        ret = IndentedString()
        ret.write('class {name} : public Fwk::PtrInterface<{name}> {{\n'.format(name=self.classname))
        ret.write('  public:\n')
        ret.set_indent(4)
        ret.write('%s() {\n' % self.classname)
        ret.write('    network_ = new ShippingNetwork();\n')
        ret.write('}\n')
        ret.set_indent(0)
        for c in classes.values():
            ret.write(c.newInstanceMethodStr(4))
        #ret.set_indent(4)
        ret.write(self.notifiee_str())
        ret.write('  protected:\n')
        ret.write('    Ptr<ShippingNetwork> network_;\n')
        ret.write(self.notifiee_protected_str())
        ret.set_indent(0)
        ret.write('};\n')
        return ret.getvalue()

classes = {}
if __name__ == "__main__":
    baseclassOpt = 'BASECLASS'
    enumOpt = 'ENUM'
    constructorOpt = 'CONSTRUCTOR'
    config = ConfigParser.ConfigParser()
    config.optionxform = lambda x: x
    config.read('engine.conf')
    sections = config.sections()

    for section in sections:
        if section == 'HEADER' or section == 'FOOTER' or section == 'META':
            continue
        if config.has_option(section, baseclassOpt):
            c = Entity(section, config.get(section, baseclassOpt))
        else:
            c = Entity(section)
        for attr, type in config.items(section):
            if attr == baseclassOpt:
                continue
            if attr.startswith(enumOpt):
                enumname, enumitems = type.split(' ', 1)
                enumitems = enumitems.split()
                e = Enum(enumname)
                for i in enumitems:
                    e.append(i)
                c.enumIs(e)
                continue
            if attr.startswith(constructorOpt):
                c.constructorContents = type;
                continue;
            (type, _, modifiers) = type.partition('$')
            a = Attr(type, attr)
            if modifiers.find('R') >= 0:
                a.isReadonly()
            if modifiers.find('C') >= 0:
                a.isCollection()
            if modifiers.find('V') >= 0:
                a.isVirtual()
            if modifiers.find('X') >= 0:
                a.isComplex()
            c.attrIs(a)
        classes[section] = c

    mng = ManagerClass()
    mng_file = open('Engine.h.new', 'w')
    mng_file.write('#ifndef ENGINE_MNG_H\n')
    mng_file.write('#define ENGINE_MNG_H\n')
    mng_file.write('#include "entities.h"\n')
    mng_file.write('#include "ShippingNetwork.h"\n')
    mng_file.write('#include "Ptr.h"\n')
    mng_file.write('#include "PtrInterface.h"\n')
    mng_file.write('namespace Shipping {\n')
    print config.get('HEADER', 'content').replace('\\t', '    ').replace('\\n', '')
    for c in classes.values():
        print 'class %s;' % c.classname
    for cn in config.get('META', 'ORDER').split():
        print classes[cn]
    print config.get('FOOTER', 'content').replace('\\t', '    ').replace('\\n', '')

    mng_file.write('%s' % mng)
    mng_file.write('}\n#endif\n')
    mng_file.close()
