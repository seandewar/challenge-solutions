// https://leetcode.com/problems/regions-cut-by-slashes
// Complexity: runtime O(n*m), space O(n*m).

class Solution {
  enum Quadrant { BOTL, BOTR, TOPL, TOPR };

  bool fill(const auto &grid, auto &visited, int x, int y, Quadrant q) {
    if (x < 0 || y < 0 || x >= grid[0].size() || y >= grid.size())
      return false;

    auto &vis_quad = visited[y * grid[0].size() + x];
    if (vis_quad[q])
      return false;

    enum Dir { LEFT, RIGHT, UP, DOWN };
    auto fill_dir = [&](Dir dir) {
      constexpr array<tuple<int, int, Quadrant, Quadrant>, 4> lookup{
          {{-1, 0, BOTR, TOPR},
           {1, 0, BOTL, TOPL},
           {0, -1, BOTL, BOTR},
           {0, 1, TOPL, TOPR}}};

      const auto [dx, dy, q1, q2] = lookup[dir];
      fill(grid, visited, x + dx, y + dy, q1);
      fill(grid, visited, x + dx, y + dy, q2);
    };
    const auto cell = grid[y][x];

    if (cell == ' ') {
      vis_quad.fill(true);
      for (auto d : {LEFT, RIGHT, UP, DOWN})
        fill_dir(d);
      return true;
    }
    if ((cell == '/') == (q == BOTL || q == TOPR))
      return false;

    vis_quad[q] = true;
    fill_dir(q == BOTL || q == TOPL ? LEFT : RIGHT);
    fill_dir(q == BOTL || q == BOTR ? DOWN : UP);
    return true;
  }

public:
  int regionsBySlashes(vector<string> &grid) {
    vector visited(grid.size() * grid[0].size(), array<bool, 4>{});

    int regions = 0;
    for (int y = 0; y < grid.size(); ++y) {
      for (int x = 0; x < grid[0].size(); ++x) {
        const auto cell = grid[y][x];
        regions += fill(grid, visited, x, y, cell == '/' ? BOTR : BOTL);
        regions += fill(grid, visited, x, y, cell == '/' ? TOPL : TOPR);
      }
    }
    return regions;
  }
};
