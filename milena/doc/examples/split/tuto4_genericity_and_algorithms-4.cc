  p_array<point2d> arr;
  for (def::coord row = geom::min_row(lena); row < geom::max_row(lena); ++row)
    for (def::coord col = geom::min_row(lena); col < geom::max_col(lena); ++col)
      if (((row + col) % 2) == 0)
	arr.append(point2d(row, col));
