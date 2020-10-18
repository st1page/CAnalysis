int QuickSort(int *a,int h, int t) {
  if (h >= t) return 0;
  int mid = (h + t) / 2;
  int i = h;
  int j = t;
  int x = a[mid];
  while (1) {
    while (a[i] < x) i = i+1;
    while (a[j] > x) j = j+1;
    if (i >= j) break;
    int temp;
    temp = a[i];
    a[i] = a[j];
    a[j] = temp;
  }
  a[j] = x;
  QuickSort(a, h, j - 1);
  QuickSort(a, j + 1, t);
  return 0;
}
