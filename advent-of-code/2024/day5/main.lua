-- Nvim 0.11; simply :source this file.
--
-- Luckily, the rules are total, so no implicit relations here; we can simply
-- solve this via sorting with a custom comparator (no graph shenanigans!)
--
-- Typical AoC fashion that this property isn't specified in the question...

local rules, line_it = {}, io.lines "input"
for line in line_it do
  if line == "" then
    break
  end
  local before, after = line:match "^(%d+)|(%d+)$"
  before, after = tonumber(before), tonumber(after)
  local befores = rules[after] or {}
  befores[#befores + 1] = before
  rules[after] = befores
end

local p1, p2 = 0, 0
for line in line_it do
  local numbers =
    vim.iter(vim.gsplit(line, ",", { plain = true })):map(tonumber):totable()
  local sorted = vim.deepcopy(numbers)
  table.sort(sorted, function(a, b)
    return vim.tbl_contains(rules[b] or {}, a)
  end)

  local score = sorted[(1 + #sorted) / 2]
  if vim.deep_equal(numbers, sorted) then
    p1 = p1 + score
  else
    p2 = p2 + score
  end
end

print("Day5: P1: " .. p1 .. ", P2: " .. p2)
