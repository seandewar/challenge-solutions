// https://leetcode.com/problems/parsing-a-boolean-expression
// Complexity: runtime O(n), space O(n).

static bool parse(const char **const p)
{
    if (**p == 't') {
        ++*p; // skip 't'
        return true;
    }
    if (**p == 'f') {
        ++*p; // skip 'f'
        return false;
    }
    if (**p == '!') {
        *p += 2; // skip '!('
        const bool ret = !parse(p);
        ++*p; // skip ')'
        return ret;
    }

    const bool and = **p == '&';
    *p += 2; // skip '&(' or '|('
    bool ret = and;
    do {
        const bool v = parse(p);
        ret = and? ret && v : ret || v;
    } while (*((*p)++) != ')'); // skip ',' or ')'
    return ret;
}

bool parseBoolExpr(const char *expr)
{
    return parse(&expr);
}
