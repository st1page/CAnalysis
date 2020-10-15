int f(int x, int y) {
  int k = x + y;
  int i;
  if (x + y == k) {
     do {
       i = i+1;
     } while (i < x);
  } else {
     i = 0;
     while (i < x) {
       i = i+1;
     }
  }
}