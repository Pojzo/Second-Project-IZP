"""
Testing script for the 2021 IZP team project
Author: kam29#4080
"""

import argparse
import os.path
import random
import subprocess

DEFAULT_RANDOMS = 0
DEFAULT_EXECUTABLE = './setcal'
DEFAULT_TIMEOUT = 5
DEFAULT_TMP_FILE = './tmp.txt'

SINGLE_SET_COMMANDS = ['empty', 'card', 'complement']
DOUBLE_SET_COMMANDS = ['union', 'intersect', 'minus', 'subseteq', 'subset', 'equals']
REL_COMMANDS = ['reflexive', 'symmetric', 'antisymmetric', 'transitive', 'function', 'domain', 'codomain']
BOTH_COMMANDS = ['injective', 'surjective', 'bijective']
SET_COMMANDS = SINGLE_SET_COMMANDS + DOUBLE_SET_COMMANDS
ALL_COMMANDS = SET_COMMANDS + REL_COMMANDS + BOTH_COMMANDS

ELEMENT_PREFIX = 'e'
MIN_ELEMENTS = 3
MAX_ELEMENTS = 100
MIN_SETS = 2
MAX_SETS = 20
MIN_RELS = 2
MAX_RELS = 20
MIN_CMDS = 10
MAX_CMDS = 100

TEST_UNIVERSUM_COMMON = 'U a b c x y z\n'
TEST_SET_COMMON = TEST_UNIVERSUM_COMMON + 'S a b x\nS\nS z y b a\n'
TEST_EMPTY_IN = TEST_SET_COMMON + 'C empty 2\nC empty 4\nC empty 3\nC empty 1\n'
TEST_EMPTY_OUT = TEST_SET_COMMON + 'false\nfalse\ntrue\nfalse\n'
TEST_CARD_IN = TEST_SET_COMMON + 'C card 1\nC card 2\nC card 3\nC card 4\n'
TEST_CARD_OUT = TEST_SET_COMMON + '6\n3\n0\n4\n'
TEST_COMPLEMENT_IN = TEST_SET_COMMON + 'C complement 4\nC complement 3\nC complement 2\nC complement 1\n'
TEST_COMPLEMENT_OUT = TEST_SET_COMMON + 'S c x\nS a b c x y z\nS c y z\nS\n'
TEST_UNION_IN = TEST_SET_COMMON + 'C union 1 1\nC union 1 2\nC union 1 3\nC union 1 4\nC union 2 1\nC union 2 2\nC union 2 3\nC union 2 4\nC union 3 1\nC union 3 2\nC union 3 3\nC union 3 4\nC union 4 1\nC union 4 2\nC union 4 3\nC union 4 4\n'
TEST_UNION_OUT = TEST_SET_COMMON + 'S a b c x y z\nS a b c x y z\nS a b c x y z\nS a b c x y z\nS a b c x y z\nS a b x\nS a b x\nS a b x y z\nS a b c x y z\nS a b x\nS\nS a b y z\nS a b c x y z\nS a b x y z\nS z y b a\nS z y b a\n'
TEST_INTERSECT_IN = TEST_SET_COMMON + 'C intersect 1 1\nC intersect 1 2\nC intersect 1 3\nC intersect 1 4\nC intersect 2 1\nC intersect 2 2\nC intersect 2 3\nC intersect 2 4\nC intersect 3 1\nC intersect 3 2\nC intersect 3 3\nC intersect 3 4\nC intersect 4 1\nC intersect 4 2\nC intersect 4 3\nC intersect 4 4\n'
TEST_INTERSECT_OUT = TEST_SET_COMMON + 'S a b c x y z\nS a b x\nS\nS a b y z\nS a b x\nS a b x\nS\nS a b\nS\nS\nS\nS\nS a b y z\nS a b\nS\nS z y b a\n'
TEST_MINUS_IN = TEST_SET_COMMON + 'C minus 1 1\nC minus 1 2\nC minus 1 3\nC minus 1 4\nC minus 2 1\nC minus 2 2\nC minus 2 3\nC minus 2 4\nC minus 3 1\nC minus 3 2\nC minus 3 3\nC minus 3 4\nC minus 4 1\nC minus 4 2\nC minus 4 3\nC minus 4 4\n'
TEST_MINUS_OUT = TEST_SET_COMMON + 'S\nS c y z\nS a b c x y z\nS c x\nS\nS\nS a b x\nS x\nS\nS\nS\nS\nS\nS y z\nS z y b a\nS\n'
TEST_SUBSETEQ_IN = TEST_SET_COMMON + 'C subseteq 1 1\nC subseteq 1 2\nC subseteq 1 3\nC subseteq 1 4\nC subseteq 2 1\nC subseteq 2 2\nC subseteq 2 3\nC subseteq 2 4\nC subseteq 3 1\nC subseteq 3 2\nC subseteq 3 3\nC subseteq 3 4\nC subseteq 4 1\nC subseteq 4 2\nC subseteq 4 3\nC subseteq 4 4\n'
TEST_SUBSETEQ_OUT = TEST_SET_COMMON + 'true\nfalse\nfalse\nfalse\ntrue\ntrue\nfalse\nfalse\ntrue\ntrue\ntrue\ntrue\ntrue\nfalse\nfalse\ntrue\n'
TEST_SUBSET_IN = TEST_SET_COMMON + 'C subset 1 1\nC subset 1 2\nC subset 1 3\nC subset 1 4\nC subset 2 1\nC subset 2 2\nC subset 2 3\nC subset 2 4\nC subset 3 1\nC subset 3 2\nC subset 3 3\nC subset 3 4\nC subset 4 1\nC subset 4 2\nC subset 4 3\nC subset 4 4\n'
TEST_SUBSET_OUT = TEST_SET_COMMON + 'false\nfalse\nfalse\nfalse\ntrue\nfalse\nfalse\nfalse\ntrue\ntrue\nfalse\ntrue\ntrue\nfalse\nfalse\nfalse\n'
TEST_EQUALS_IN = TEST_SET_COMMON + 'S\nC equals 1 1\nC equals 1 2\nC equals 1 3\nC equals 1 4\nC equals 2 1\nC equals 2 2\nC equals 2 3\nC equals 2 4\nC equals 3 1\nC equals 3 2\nC equals 3 3\nC equals 3 4\nC equals 4 1\nC equals 4 2\nC equals 4 3\nC equals 4 4\nC equals 4 5\nC equals 5 3\n'
TEST_EQUALS_OUT = TEST_SET_COMMON + 'S\ntrue\nfalse\nfalse\nfalse\nfalse\ntrue\nfalse\nfalse\nfalse\nfalse\ntrue\nfalse\nfalse\nfalse\nfalse\ntrue\nfalse\ntrue\n'
TEST_REL_COMMON = TEST_UNIVERSUM_COMMON + 'R (a a) (a b) (a c) (a x) (a y) (a z) (b a) (b b) (b c) (b x) (b y) (b z) (c a) (c b) (c c) (c x) (c y) (c z) (x a) (x b) (x c) (x x) (x y) (x z) (y a) (y b) (y c) (y x) (y y) (y z) (z a) (z b) (z c) (z x) (z y) (z z)\nR\n'
TEST_REFLEXIVE_COMMON = TEST_REL_COMMON + 'R (a x) (a a) (b x) (x x) (b b)\nR (a x) (a a)\n'
TEST_REFLEXIVE_IN = TEST_REFLEXIVE_COMMON + 'C reflexive 2\nC reflexive 3\nC reflexive 4\nC reflexive 5\n'
TEST_REFLEXIVE_OUT = TEST_REFLEXIVE_COMMON + 'true\ntrue\ntrue\nfalse\n'
TEST_SYMMETRIC_COMMON = TEST_REL_COMMON + 'R (a x) (a a) (c x) (x x) (x a) (x c)\nR (b z) (b b)\n'
TEST_SYMMETRIC_IN = TEST_SYMMETRIC_COMMON + 'C symmetric 2\nC symmetric 3\nC symmetric 4\nC symmetric 5\n'
TEST_SYMMETRIC_OUT = TEST_SYMMETRIC_COMMON + 'true\ntrue\ntrue\nfalse\n'
TEST_ANTISYMMETRIC_COMMON = TEST_REL_COMMON + 'R (a x) (c x) (z b) (y a) (y z)\nR (b z) (b b) (z b)\n'
TEST_ANTISYMMETRIC_IN = TEST_ANTISYMMETRIC_COMMON + 'C antisymmetric 2\nC antisymmetric 3\nC antisymmetric 4\nC antisymmetric 5\n'
TEST_ANTISYMMETRIC_OUT = TEST_ANTISYMMETRIC_COMMON + 'false\ntrue\ntrue\nfalse\n'
TEST_TRANSITIVE_COMMON = TEST_REL_COMMON + 'R (a x) (a a) (c a) (c x) (x x) (x a) (c c) (x c) (a c)\nR (b z) (z y) (y b)\n'
TEST_TRANSITIVE_IN = TEST_TRANSITIVE_COMMON + 'C transitive 2\nC transitive 3\nC transitive 4\nC transitive 5\n'
TEST_TRANSITIVE_OUT = TEST_TRANSITIVE_COMMON + 'true\ntrue\ntrue\nfalse\n'
TEST_FUNCTION_COMMON = TEST_REL_COMMON + 'R (a x) (c x) (x x) (z a)\nR (b z) (b y)\n'
TEST_FUNCTION_IN = TEST_FUNCTION_COMMON + 'C function 2\nC function 3\nC function 4\nC function 5\n'
TEST_FUNCTION_OUT = TEST_FUNCTION_COMMON + 'false\ntrue\ntrue\nfalse\n'
TEST_DOMAIN_COMMON = TEST_REL_COMMON + 'R (a x) (c x) (x x) (z a)\nR (b z) (b y)\n'
TEST_DOMAIN_IN = TEST_DOMAIN_COMMON + 'C domain 2\nC domain 3\nC domain 4\nC domain 5\n'
TEST_DOMAIN_OUT = TEST_DOMAIN_COMMON + 'S x y z a b c\nS\nS a x c z\nS b\n'
TEST_CODOMAIN_COMMON = TEST_REL_COMMON + 'R (a c) (c x) (x x) (z a)\nR (b z) (c z) (a z)\n'
TEST_CODOMAIN_IN = TEST_CODOMAIN_COMMON + 'C codomain 2\nC codomain 3\nC codomain 4\nC codomain 5\n'
TEST_CODOMAIN_OUT = TEST_CODOMAIN_COMMON + 'S x y z a b c\nS\nS a x c\nS z\n'

TEST_MAPPING_COMMON = 'U a b c x y z w\nS a b c\nS x y z\nS x y\nS x y z w\nS\nR (x y) (a x) (b z) (c y)\nR (a x) (b a) (c z)\nR (a x) (b y)\nR (a x) (a y) (b y) (c z)\nR\nR (a x) (b y) (c x)\nR (a x) (b y) (c z)\n'
TEST_MAPPING_OUT_COMMON = TEST_MAPPING_COMMON + 'false\nfalse\nfalse\nfalse\nfalse\ntrue\nfalse\nfalse\nfalse\n'
TEST_INJECTIVE_IN = TEST_MAPPING_COMMON + 'C injective 7 2 3\nC injective 8 2 3\nC injective 9 2 3\nC injective 10 2 3\nC injective 11 2 3\nC injective 11 6 6\nC injective 13 2 6\nC injective 13 6 3\nC injective 13 6 6\nC injective 12 2 3\nC injective 12 2 4\nC injective 13 2 3\nC injective 13 2 5\n'
TEST_INJECTIVE_OUT = TEST_MAPPING_OUT_COMMON + 'false\nfalse\ntrue\ntrue\n'
TEST_SURJECTIVE_IN = TEST_MAPPING_COMMON + 'C surjective 7 2 3\nC surjective 8 2 3\nC surjective 9 2 3\nC surjective 10 2 3\nC surjective 11 2 3\nC surjective 11 6 6\nC surjective 13 2 6\nC surjective 13 6 3\nC surjective 13 6 6\nC surjective 12 2 3\nC surjective 12 2 4\nC surjective 13 2 3\nC surjective 13 2 5\n'
TEST_SURJECTIVE_OUT = TEST_MAPPING_OUT_COMMON + 'false\ntrue\ntrue\nfalse\n'
TEST_BIJECTIVE_IN = TEST_MAPPING_COMMON + 'C bijective 7 2 3\nC bijective 8 2 3\nC bijective 9 2 3\nC bijective 10 2 3\nC bijective 11 2 3\nC bijective 11 6 6\nC bijective 13 2 6\nC bijective 13 6 3\nC bijective 13 6 6\nC bijective 12 2 3\nC bijective 12 2 4\nC bijective 13 2 3\nC bijective 13 2 5\n'
TEST_BIJECTIVE_OUT = TEST_MAPPING_OUT_COMMON + 'false\nfalse\ntrue\nfalse\n'

DIRECT_TESTS = [
        ('empty', (TEST_EMPTY_IN, TEST_EMPTY_OUT)),
        ('card', (TEST_CARD_IN, TEST_CARD_OUT)),
        ('complement', (TEST_COMPLEMENT_IN, TEST_COMPLEMENT_OUT)),
        ('union', (TEST_UNION_IN, TEST_UNION_OUT)),
        ('intersect', (TEST_INTERSECT_IN, TEST_INTERSECT_OUT)),
        ('minus', (TEST_MINUS_IN, TEST_MINUS_OUT)),
        ('subseteq', (TEST_SUBSETEQ_IN, TEST_SUBSETEQ_OUT)),
        ('subset', (TEST_SUBSET_IN, TEST_SUBSET_OUT)),
        ('equals', (TEST_EQUALS_IN, TEST_EQUALS_OUT)),
        ('reflexive', (TEST_REFLEXIVE_IN, TEST_REFLEXIVE_OUT)),
        ('symmetric', (TEST_SYMMETRIC_IN, TEST_SYMMETRIC_OUT)),
        ('antisymmetric', (TEST_ANTISYMMETRIC_IN, TEST_ANTISYMMETRIC_OUT)),
        ('transitive', (TEST_TRANSITIVE_IN, TEST_TRANSITIVE_OUT)),
        ('function', (TEST_FUNCTION_IN, TEST_FUNCTION_OUT)),
        ('domain', (TEST_DOMAIN_IN, TEST_DOMAIN_OUT)),
        ('codomain', (TEST_CODOMAIN_IN, TEST_CODOMAIN_OUT)),
        ('injective', (TEST_INJECTIVE_IN, TEST_INJECTIVE_OUT)),
        ('surjective', (TEST_SURJECTIVE_IN, TEST_SURJECTIVE_OUT)),
        ('bijective', (TEST_BIJECTIVE_IN, TEST_BIJECTIVE_OUT))]

def ParseArguments():
    parser = argparse.ArgumentParser(description='Testing environment for IZP2 project')
    parser.add_argument('--randoms', '-r', type=int, default=DEFAULT_RANDOMS, help=('number of randomly generated tests to run (default: ' + str(DEFAULT_RANDOMS) + ', only direct tests are run)' ))
    parser.add_argument('--executable', '-x', default=DEFAULT_EXECUTABLE, help=('path to the project executable (default: ' + DEFAULT_EXECUTABLE + ')'))
    group = parser.add_mutually_exclusive_group()
    group.add_argument('--no-sets', action='store_true', help='no lines with sets or set commands will be used in the tests')
    group.add_argument('--no-relations', action='store_true', help='no lines with relations or relation commands will be used in the tests')
    group = parser.add_mutually_exclusive_group()
    group.add_argument('--include', '-i', nargs='+', help='use only specified commands in the tests')
    group.add_argument('--exclude', '-e', nargs='+', help='use all except specified commands in the tests')
    parser.add_argument('--quiet', '-q', action='store_true', help='print only names of failed tests and final summary')
    parser.add_argument('--randoms-only', action='store_true', help='no direct tests will be run, only random tests')
    parser.add_argument('--timeout', type=int, default=DEFAULT_TIMEOUT, help=('number of seconds after which the test is considered failed if still running (default: ' + str(DEFAULT_TIMEOUT) + ')'))
    parser.add_argument('--tmp-file', default=DEFAULT_TMP_FILE, help=('path that will be used to create tmp file for the test inputs (default: ' + DEFAULT_TMP_FILE + ')'))
    
    args = parser.parse_args()

    if args.randoms < 0:
        raise RuntimeError('The --randoms argument muset have value greater than or equal to 0 (current value: ' + str(args.randoms) + ')')
    if args.timeout < 1:
        raise RuntimeError('The --timeout argument must have value greater than 0 (current value: ' + str(args.timeout) + ')')
    if not os.path.isfile(args.executable):
        raise RuntimeError('The --executable argument must point to an existing file (current value: ' + args.executable + ')')
    if os.path.isfile(args.tmp_file) and not args.quiet:
        print('WARNING: The --tmp-file argument points to an existing file which will be overwritten')

    if args.include:
        cmds = args.include
    elif args.exclude:
        cmds = args.exclude
    else:
        cmds = []

    args.include = [] if args.include is None else args.include
    args.exclude = [] if args.exclude is None else args.exclude

    for cmd in cmds:
        if cmd not in ALL_COMMANDS:
            raise RuntimeError('The --include and --exclude arguments must contain only valid commands (invalid command: ' + cmd + ')')

    if args.no_sets:
        for item in args.include:
            if not args.quiet and (item in SET_COMMANDS or item in BOTH_COMMANDS):
                print('WARNING: The --include or --exclude argument contains set command (\'' + item + '\'), which will be ignored because of the --no-sets argument')
    elif args.no_relations:
        for item in args.include:
            if not args.quiet and (item in REL_COMMANDS or item in BOTH_COMMANDS):
                print('WARNING: The --include or --exclude argument contains relation command (\'' + item + '\'), which will be ignored because of the --no-relations argument')

    return args

def GenerateUniversum():
    output = []
    elements_count = random.randint(MIN_ELEMENTS, MAX_ELEMENTS)
    for i in range(elements_count):
        output.append(ELEMENT_PREFIX + str(i))
    return output

def GenerateSet(universum):
    elements_count = random.randint(0, len(universum))
    sample = random.sample(universum, elements_count)
    return sample

def GenerateRel(universum):
    elements_count = random.randint(0, len(universum))
    sampleA = random.sample(universum, elements_count)
    sampleB = random.sample(universum, elements_count)
    output = []
    for i in range(elements_count):
        output.append((sampleA[i], sampleB[i]))
    return output

def GetUsedCommands(args):
    output = []
    if args.no_sets:
        for item in args.include:
            if item in REL_COMMANDS:
                output.append(item)
        if args.include == []:
            for item in REL_COMMANDS:
                if item not in args.exclude:
                    output.append(item)
    elif args.no_relations:
        for item in args.include:
            if item in SET_COMMANDS:
                output.append(item)
        if args.include == []:
            for item in SET_COMMANDS:
                if item not in args.exclude:
                    output.append(item)
    else:
        for item in args.include:
            output.append(item)
        if args.include == []:
            for item in ALL_COMMANDS:
                if item not in args.exclude:
                    output.append(item)
    return output

def CalculateSingleSetCommand(cmd, universum, set_instance):
    if cmd == 'empty':
        output = 'true' if len(set_instance) == 0 else 'false'
    elif cmd == 'card':
        output = str(len(set_instance))
    elif cmd == 'complement':
        output = 'S'
        for item in universum:
            if item not in set_instance:
                output += ' ' + item
    else:
        raise RuntimeError('Unrecognized single set command')
    return output

def CalculateDoubleSetCommand(cmd, universum, set_instanceA, set_instanceB):
    if cmd == 'union':
        output = 'S'
        for item in universum:
            if item in set_instanceA or item in set_instanceB:
                output += ' ' + item
    elif cmd == 'intersect':
        output = 'S'
        for item in set_instanceA:
            if item in set_instanceB:
                output += ' ' + item
    elif cmd == 'minus':
        output = 'S'
        for item in set_instanceA:
            if item not in set_instanceB:
                output += ' ' + item
    elif cmd == 'subseteq':
        output = 'true'
        for item in set_instanceA:
            if item not in set_instanceB:
                output = 'false'
                break
    elif cmd == 'subset':
        output = 'true'
        for item in set_instanceA:
            if item not in set_instanceB:
                output = 'false'
                break
        if output == 'true' and len(set_instanceA) == len(set_instanceB):
            output = 'false'
    elif cmd == 'equals':
        output = 'true'
        for item in set_instanceA:
            if item not in set_instanceB:
                output = 'false'
                break
        if output == 'true' and len(set_instanceA) != len(set_instanceB):
            output = 'false'
    else:
        raise RuntimeError('Unexpected double set command')
    return output

def CalculateRelCommand(cmd, universum, rel_instance):
    if cmd == 'reflexive':
        result = []
        for item in rel_instance:
            if item[0] not in result:
                result.append(item[0])
            if item[1] not in result:
                result.append(item[1])
        output = 'true'
        for item in result:
            found = False
            for pair in rel_instance:
                if pair[0] == item and pair[1] == item:
                    found = True
                    break
            if not found:
                output = 'false'
                break
    elif cmd == 'symmetric':
        output = 'true'
        for item in rel_instance:
            found = False
            for pair in rel_instance:
                if pair[0] == item[1] and pair[1] == item[0]:
                    found = True
                    break
            if not found:
                output = 'false'
                break
    elif cmd == 'antisymmetric':
        output = 'true'
        for item in rel_instance:
            found = False
            for pair in rel_instance:
                if pair[0] == item[1] and pair[1] == item[0]:
                    found = True
                    break
            if found:
                output = 'false'
                break
    elif cmd == 'transitive':
        output = 'true'
        for itemA in rel_instance:
            for itemB in rel_instance:
                if itemA[1] == itemB[0]:
                    found = False
                    for pair in rel_instance:
                        if pair[0] == itemA[0] and pair[1] == itemB[1]:
                            found = True
                            break 
                    if not found:
                        output = 'false'
                        break
            if output == 'false':
                break

    elif cmd == 'function':
        result = []
        output = 'true'
        for item in rel_instance:
            if item in result:
                output = 'false'
                break
            else:
                result.append(item)
    elif cmd == 'domain':
        result = []
        for item in rel_instance:
            if item[0] not in result:
                result.append(item[0])
        if result == []:
            output = 'S'
        else:
            output = 'S ' + ' '.join(result)
    elif cmd == 'codomain':
        result = []
        for item in rel_instance:
            if item[1] not in result:
                result.append(item[1])
        if result == []:
            output = 'S'
        else:
            output = 'S ' + ' '.join(result)
    else:
        raise RuntimeRrror('Unexpected relation command')
    return output

def IsMapping (rel, domain, codomain):
    for itemA in domain:
        found = False
        for itemB in rel:
            if itemA == itemB[0]:
                found = True
                break
        if not found:
            return False
    for itemA in rel:
        if itemA[0] not in domain:
            return False
        if itemA[1] not in codomain:
            return False
        found = False
        for itemB in rel:
            if itemA[0] == itemB[0]:
                if not found:
                    found = True
                else:
                    return False
    return True

def CalculateCombinedCommand(cmd, rel_instance, setA_instance, setB_instance):
    if cmd == 'injective':
        if not IsMapping(rel_instance, setA_instance, setB_instance):
            output = 'false'
        else:
            codomain = []
            output = 'true'
            for item in rel_instance:
                if item[1] in codomain:
                    output = 'false'
                    break
                else:
                    codomain.append(item[1])
    elif cmd == 'surjective':
        if not IsMapping(rel_instance, setA_instance, setB_instance):
            output = 'false'
        else:
            codomain = []
            for item in rel_instance:
                if item[1] not in codomain:
                    codomain.append(item[1])
            if len(setB_instance) == len(codomain):
                output = 'true'
            else:
                output = 'false'
    elif cmd == 'bijective':
        if not IsMapping(rel_instance, setA_instance, setB_instance):
            output = 'false'
        elif CalculateCombinedCommand('injective', rel_instance, setA_instance, setB_instance) == 'true' and CalculateCombinedCommand('surjective', rel_instance, setA_instance, setB_instance) == 'true':
            output = 'true'
        else:
            output = 'false'
    else:
        raise RuntimeRrror('Unexpected relation command')
    return output

def GenerateCommand(universum, sets, rels, cmds):
    cmd = random.choice(cmds)
    output = 'C ' + cmd + ' '
    if cmd in SINGLE_SET_COMMANDS:
        set_instance = random.choice(sets)
        output += str(set_instance[0])
        result = CalculateSingleSetCommand(cmd, universum, set_instance[1])
    elif cmd in DOUBLE_SET_COMMANDS:
        set_instanceA = random.choice(sets)
        set_instanceB = random.choice(sets)
        output += str(set_instanceA[0]) + ' ' + str(set_instanceB[0])
        result = CalculateDoubleSetCommand(cmd, universum, set_instanceA[1], set_instanceB[1])
    elif cmd in REL_COMMANDS:
        rel_instance = random.choice(rels)
        output += str(rel_instance[0])
        result = CalculateRelCommand(cmd, universum, rel_instance[1])
    elif cmd in BOTH_COMMANDS:
        rel_instance = random.choice(rels)
        set_instanceA = random.choice(sets)
        set_instanceB = random.choice(sets)
        output += str(rel_instance[0]) + ' ' + str(set_instanceA[0]) + ' ' + str(set_instanceB[0])
        result = CalculateCombinedCommand(cmd, rel_instance[1], set_instanceA[1], set_instanceB[1])
    else:
        raise RuntimeError('Unexpected command generated')
    return (output, result)

def GenerateTest(args):
    universum = GenerateUniversum()
    output = 'U ' + ' '.join(universum) + '\n'
    result = output
    counter = 1
    sets = [(counter, universum)]
    rels = []

    if not args.no_sets:
        sets_count = random.randint(MIN_SETS, MAX_SETS)
        for i in range(sets_count):
            counter = counter + 1
            set_instance = GenerateSet(universum)
            sets.append((counter, set_instance))
            if set_instance == []:
                set_str = 'S\n'
            else:
                set_str = 'S ' + ' '.join(set_instance) + '\n'
            output += set_str
            result += set_str

    if not args.no_relations:
        rels_count = random.randint(MIN_RELS, MAX_RELS)
        for i in range(rels_count):
            counter = counter + 1
            rel_instance = GenerateRel(universum)
            rels.append((counter, rel_instance))
            if rel_instance == []:
                rel_str = 'R\n'
            else:
                rel_str = 'R'
                for item in rel_instance:
                    rel_str += ' (' + item[0] + ' ' + item[1] + ')'
                rel_str += '\n'
            output += rel_str
            result += rel_str

    cmds = GetUsedCommands(args)
    cmds_count = random.randint(MIN_CMDS, MAX_CMDS)
    for i in range(cmds_count):
        cmd = GenerateCommand(universum, sets, rels, cmds)
        output += cmd[0] + '\n'
        result += cmd[1] + '\n'

    return (output, result)

def FindInconsistencies(out, ref):
    result = []
    both_lines = min(len(out), len(ref))

    for i in range(both_lines):
        out_line = out[i]
        ref_line = ref[i]
        if len(ref_line) == 0 or ref_line[0] not in ['S', 'R']:
            if ref_line != out_line:
                result.append(i)
        elif ref_line[0] == 'S':
            ref_items = ref_line.split(' ')
            out_items = out_line.split(' ')
            if ref_items[0] != out_items[0] or len(ref_items) != len(out_items):
                result.append(i)
            else:
                for item in ref_items:
                    if item not in out_items:
                        result.append(i)
                        break
        else:
            if out_line[0] != 'R' or (ref_line == 'R' and out_line != 'R'):
                result.append(i)
            else:
                ref_items = ref_line[2:].split(' ')
                out_items = out_line[2:].split(' ')
                if len(ref_items) != len(out_items):
                    result.append(i)
                else:
                    ref_pairs = []
                    out_pairs = []
                    if ref_items == [] or ref_items == ['']:
                        if out_items != [] and out_items != ['']:
                            result.append(i)
                    else:
                        for i in range(0, len(ref_items), 2):
                            ref_pairs.append((ref_items[i], ref_items[i+1]))
                            out_pairs.append((out_items[i], out_items[i+1]))
                        for pair in ref_pairs:
                            if pair not in out_pairs:
                                result.append(i)
                                break

    if len(out) != len(ref):
        max_lines = max(len(out), len(ref))
        for i in range(both_lines, max_lines):
            result.append(i)
    return result

def RunTest(args, test, name):
    with open(args.tmp_file, 'w', newline='\n') as f:
        f.write(test[0])
    result = subprocess.run([args.executable, args.tmp_file], timeout=args.timeout, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if result.returncode != 0:
        print('Test \'' + name + '\' FAILED')
        if not args.quiet:
            print('Actual exit code: ' + str(result.returncode))
            print('----------')
            print('Input:')
            print(test[0])
            if result.stderr:
                print('----------')
                print('Actual error output:')
                print(result.stderr.decode())
            print('----------')
        return False
    else:
        out = result.stdout.decode().split('\n')
        ref = test[1].split('\n')
        inlines = test[0].split('\n')
        inconsistencies = FindInconsistencies(out, ref)
        if inconsistencies != []:
            print('Test \'' + name + '\' FAILED')
            if not args.quiet:
                print('----------')
                print('Input:')
                for i in range(len(inlines)):
                    if i in inconsistencies:
                        print(str(i+1) + ': ' + inlines[i] + ' # MISSMATCH')
                    else:
                        print(str(i+1) + ': ' + inlines[i])
                print('----------')
                print('Expected output:')
                for i in range(len(ref)):
                    if i in inconsistencies:
                        print(str(i+1) + ': ' + ref[i] + ' # MISSMATCH')
                    else:
                        print(str(i+1) + ': ' + ref[i])
                print('----------')
                print('Actual output:')
                for i in range(len(out)):
                    if i in inconsistencies:
                        print(str(i+1) + ': ' + out[i] + ' # MISSMATCH')
                    else:
                        print(str(i+1) + ': ' + out[i])
                if result.stderr:
                    print('----------')
                    print('Actual error output:')
                    print(result.stderr.decode())
                print('----------')
            return False
        else:
            if not args.quiet:
                print('Test \'' + name + '\' PASSED')
            return True

args = ParseArguments()
passed = 0
failed = 0
skiped = 0
if not args.quiet:
    print('------------------------------')

if args.randoms_only:
    if not args.quiet:
        print('DIRECT TESTS SKIPED')
else:
    for t in DIRECT_TESTS:
        if t[0] in GetUsedCommands(args):
            result = RunTest(args, t[1], t[0])
            if result:
                passed = passed + 1
            else:
                failed = failed + 1
        else:
            skiped = skiped + 1
            if not args.quiet:
                print('Test \'' + t[0] + '\' SKIPED')

if args.randoms > 0:
    for i in range(args.randoms):
        test = GenerateTest(args)
        result = RunTest(args, test, 'random-' + str(i+1))
        if result:
            passed = passed + 1
        else:
            failed = failed + 1
elif not args.quiet:
    print('RANDOM TESTS SKIPED')

print('---------- SUMMARY ----------')
print('PASSED: ' + str(passed))
print('FAILED: ' + str(failed))

