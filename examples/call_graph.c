void f1();
void f2();
void f3();
void f5();
void f1() {
  f2();
  f3();
}
void f2() {
  f2();
}
void f3() {
  f1();
}
void f4() {

}
void f5() {
  f1();
  f2();
  f3();
  f4();
  f5();
}