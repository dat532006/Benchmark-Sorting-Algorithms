# Đồ án: Benchmark Sorting Algorithms

## Thông tin chung

- **Môn học:** Cấu trúc dữ liệu và Giải thuật (DSA) — Học kỳ 2, năm học 2025–2026
- **Sinh viên:**
  - Nguyễn Đức Đạt — MSSV: 24120037
  - Đỗ Quốc Đại — MSSV: 24120030

---

## 1. Thuật toán tốt nhất — Benchmark 1

### Bài A — Integer Sort

**Thuật toán: Radix Sort LSD, cơ số 256 (8 bit/lượt), 4 lượt phủ hết 32 bit.**

Số nguyên có dấu được ánh xạ về khóa unsigned giữ nguyên thứ tự bằng cách lật bit dấu: `getKey(x) = (unsigned)x ^ 0x80000000`. Nhờ vậy `INT_MIN` → `0`, `INT_MAX` → `0xFFFFFFFF`, sắp xếp như unsigned vẫn đúng thứ tự signed. Mỗi lượt là một counting sort ổn định trên 8 bit (mảng đếm 256 phần tử + prefix sum), nên radix tổng thể ổn định, bảo toàn trùng lặp đúng yêu cầu.

**Tối ưu hóa:** `sync_with_stdio(false)` + `cin.tie(NULL)`; mảng đếm tĩnh 256 phần tử tái dùng qua các lượt; một buffer `temp` cấp phát một lần; xuất bằng `'\n'` thay `endl` (không flush).

**Lý do tốt nhất:** Độ phức tạp **O(4·(n+256)) = O(n)** _tất định_, không phụ thuộc phân bố dữ liệu. So với các sort so sánh O(n·log n) (≈ 17·n phép so sánh khi n=10⁵), radix chỉ quét 4 lượt tuyến tính và counting sort thân thiện cache. Quan trọng nhất: **không tồn tại input đối kháng nào làm chậm radix** — khác hẳn quicksort (O(n²) trên mảng đã sắp/toàn bằng nhau) hay insertion/bubble (O(n²) trên mảng nghịch đảo). Vì xếp hạng dựa trên *thời gian lớn nhất qua mọi test*, một thuật toán không có worst-case là lựa chọn an toàn và nhanh nhất trong các phương án đã thử ở lần 1.

### Bài B — Lexicographic Sort

**Thuật toán: Quicksort tại chỗ, chọn pivot median-of-three, phân hoạch hai con trỏ kiểu Hoare.**

Pivot lấy trung vị của `arr[left]`, `arr[mid]`, `arr[right]`; ba phần tử này được sắp trước khi phân hoạch, vừa giảm xác suất pivot xấu vừa tạo "lính canh" giúp giảm kiểm tra biên trong vòng lặp.

**Tối ưu hóa:** Hoán vị bằng `arr[i].swap(arr[j])` — `std::string::swap` chỉ tráo con trỏ/buffer nội bộ trong **O(1)**, tránh sao chép chuỗi dài tới 100 ký tự. Với n=10⁵, hoán vị bằng copy sẽ cộng thêm hệ số L=100 vào tổng chi phí. Sắp tại chỗ, độ sâu đệ quy trung bình O(log n); I/O nhanh như bài A.

**Lý do tốt nhất:** Trên dữ liệu lần 1 (chuỗi gần ngẫu nhiên), quicksort đạt trung bình O(n·log n) phép so sánh, và mỗi phép so sánh hai chuỗi ngẫu nhiên dừng rất sớm tại ký tự phân biệt đầu tiên nên chi phí thực tế mỗi compare ≈ O(1). Trong các cài đặt đã thử (insertion sort, std::sort sao chép chuỗi, quicksort copy), bản này thắng nhờ **kết hợp median-of-three giảm worst-case với swap O(1)** loại bỏ hoàn toàn chi phí sao chép chuỗi — yếu tố tốn kém nhất khi xử lý 10⁵ chuỗi.

### Bài C — Length-aware Lexicographic String Sort

**Thuật toán: Quicksort median-of-three + swap O(1) như bài B, nhưng dùng comparator `isBetter` hai khóa: độ dài tăng dần, rồi mới so sánh lexicographic khi cùng độ dài.**

```cpp
bool isBetter(a, b) { return a.length()!=b.length() ? a.length()<b.length() : a<b; }
```

**Tối ưu hóa:** Comparator **chập mạch theo độ dài**: khi hai chuỗi khác độ dài (trường hợp phổ biến), hàm trả về ngay trong O(1) chỉ bằng so sánh `length()`, *không* duyệt ký tự nào. Chỉ các chuỗi cùng độ dài mới phải so sánh lexicographic đầy đủ. Vẫn dùng `swap` O(1) và I/O nhanh.

**Lý do tốt nhất:** Bài này TL cực chặt (**1 giây, n≤10⁴**) nên mọi thuật toán O(n²) đều TLE. Quicksort cho O(n·log n) phép so sánh, và comparator chập mạch khiến đa số phép so sánh chỉ tốn O(1) (so độ dài), đẩy chi phí thực tế xuống thấp hơn nhiều so với cài đặt luôn ghép chuỗi `(len, str)` rồi so sánh, hoặc các sort O(n²). Đây là phương án nhanh nhất trong các bản đã thử ở lần 1 mà vẫn an toàn trong giới hạn 1 giây.

---

## 2. Chiến lược sinh test case (`test_gen.cpp`)

### Cách thức sinh test

Chương trình nhận 2 đối số: tên bài và số thứ tự test (1–5). Output in ra stdout đúng format input của bài tương ứng. Seed Mersenne Twister cố định để đảm bảo kết quả tái tạo được.

### Thuật toán mục tiêu và lý do

#### Bài A — Integer Sort (n≤10⁵, signed 32-bit, TL=10s)

| Test | Input | Mục tiêu | Lý do tăng thời gian |
|------|-------|----------|----------------------|
| 1 | Mảng giảm dần n→1 | Insertion Sort, Bubble Sort | Mảng nghịch đảo hoàn toàn tạo O(n²) comparisons và swaps |
| 2 | Mảng tăng dần 1→n | Quicksort pivot đầu/cuối | Pivot luôn là min → recursion depth O(n), partition lệch |
| 3 | Tất cả bằng 42 | Quicksort không 3-way | Mọi phần tử = pivot → partition luôn lệch tối đa → O(n²) |
| 4 | INT_MAX xen kẽ INT_MIN | QS dùng (a+b)/2; tải I/O | Overflow trung bình (bẫy đúng/sai); `INT_MIN` 11 ký tự → tối đa byte I/O cho radix/merge |
| 5 | Organ-pipe pattern | Quicksort median-of-three | Median-of-three chọn pivot tệ trên pattern này |

#### Bài B — Lexicographic Sort (n≤10⁵, len∈[10,100], TL=10s)

| Test | Input | Mục tiêu | Lý do tăng thời gian |
|------|-------|----------|----------------------|
| 1 | Dài 100, prefix 99 'a', khác ký tự cuối | Merge Sort, MSD/multikey radix | Mỗi compare duyệt ~100 ký tự; MSD đệ quy sâu ~99 tầng |
| 2 | Dài 100, tất cả giống hệt "aaa...a" | Merge Sort, MSD/multikey radix | Compare luôn bằng → duyệt hết 100 ký tự; MSD đệ quy tới depth L → max work |
| 3 | Reverse lexi, prefix chung 94 'a' | Merge Sort, MSD radix | Prefix chung dài → mỗi compare O(L) (do L-factor, KHÔNG phải O(n²)) |
| 4 | Prefix 95 'a', suffix 5 ngẫu nhiên | Merge Sort, MSD radix | Buộc duyệt 95% chuỗi trước khi phân biệt → compare O(L) |
| 5 | Xen kẽ "aaa...aaa" và "aaa...aab" | Merge Sort, MSD radix | Khác đúng ký tự cuối → compare duyệt hết 100 ký tự |

#### Bài C — Length-aware Lexi Sort (n≤10⁴, len∈[10,100], TL=1s!)

Bài này có TL rất chặt (1 giây), nên đặc biệt hiệu quả cho adversarial testing. Với O(n²·L) = 10⁴×10⁴×100 = 10¹⁰, bất kỳ thuật toán O(n²) nào cũng TLE.

| Test | Input | Mục tiêu | Lý do tăng thời gian |
|------|-------|----------|----------------------|
| 1 | Cùng dài 100, prefix 99 'a', khác cuối | Merge Sort (bucket/comparator) | Cùng len → fallback lexi → mỗi compare duyệt 99 ký tự → O(n·log n·L) |
| 2 | Cùng dài 100, giống hệt nhau | Merge Sort, MSD radix | Compare luôn bằng → duyệt hết 100 ký tự; MSD đệ quy tới depth L → max work |
| 3 | Cùng dài 100, prefix chung 95 'a' | Merge Sort, MSD radix | Prefix chung dài → mỗi compare O(L) (do L-factor, KHÔNG phải O(n²)) |
| 4 | Nửa dài 100, nửa dài 10, xen kẽ | Merge Sort sao chép chuỗi | Merge chép chuỗi dài vào temp tốn O(L)/phần tử; length-first short-circuit nên chỉ hại bản copy |
| 5 | Cùng dài 100, prefix 98 'a', suffix 2 ngẫu nhiên | Merge Sort, MSD radix | Cùng len → lexi → duyệt 98% chuỗi mỗi compare |

**Độ phức tạp (B, C):** nạn nhân chính là **Merge Sort — O(n·log n·L)** (thừa số `log n` từ số compare). **MSD/multikey radix — O(n·L)** (không có `log n`) chỉ chậm ~L lần so với ngẫu nhiên, vẫn nhanh.

### Lý do chọn thuật toán mục tiêu

Sau khi đọc các bài nộp Benchmark 1, các thuật toán phổ biến gồm: **bài A** — Radix Sort base-256 và Merge Sort (ping-pong buffer); **bài B** — Multikey/MSD 3-way radix quicksort (median-of-three + cutoff insertion sort) và Merge Sort (trên chỉ số hoặc sao chép chuỗi); **bài C** — Merge Sort comparator hai khóa và phương án bucket theo độ dài rồi merge sort từng nhóm.

Vì phần lớn dùng Merge Sort (O(n·log n) đảm bảo) hoặc Radix/MSD (O(n·L)) — *không có* worst-case O(n²) — bộ test không trông cậy vào việc làm bùng nổ O(n²), mà **tối đa hóa hệ số chi phí của mỗi phép so sánh / mỗi tầng đệ quy, tức độ dài chuỗi L**. Các test "prefix dài giống nhau" (99–100 ký tự `a`) buộc mọi compare phải duyệt gần hết L ký tự mới phân biệt, đồng thời buộc MSD/multikey radix đệ quy sâu tới ~L tầng → nhân thời gian chạy lên ~L≈100 lần; test "toàn bộ giống hệt" là worst-case chung cho cả hai họ. Với bài C (TL=1s) chính hệ số L này đẩy bài chậm tới sát ngưỡng TLE. Các test mảng nghịch đảo / organ-pipe / toàn bằng nhau ở bài A được giữ để chặn các bài còn dùng Insertion Sort hay Quicksort naive nếu có.

---

## 3. Thuật toán tốt nhất — Benchmark 2

### Bài A — Radix cơ số 2¹⁶, giảm còn 2 lượt

Vẫn là Radix Sort LSD nhưng nâng cơ số từ 256 lên **65536 (16 bit/lượt)**, nên phủ hết 32 bit chỉ cần **2 lượt thay vì 4** — giảm một nửa số lần quét toàn mảng (việc di chuyển dữ liệu là nút cổ chai). Phép lật bit dấu được dời ra **lúc đọc/ghi I/O** (`a[i] = (unsigned)val ^ 0x80000000`), loại bỏ hoàn toàn việc tính lại khóa bên trong vòng sort (lần 1 tính `getKey()` 2 lần/phần tử/lượt). Counting vẫn ổn định nhờ duyệt ngược `temp[--count[...]]`. Đánh đổi: mảng đếm 65536 (256 KB) lớn hơn nhưng vẫn nằm trong L2 cache, chi phí khởi tạo không đáng kể so với n=10⁵. Kết quả: nhanh ~2× so với bản 4 lượt.

### Bài B — Merge Sort trên chỉ số (index sort)

Chuyển từ quicksort sang **merge sort**, và sắp trên mảng chỉ số `idx` thay vì trực tiếp trên chuỗi.

- **O(n·log n) đảm bảo, không còn worst-case O(n²)** → miễn nhiễm các test đối kháng (đã sắp, toàn giống nhau, organ-pipe) vốn đánh quicksort xuống O(n²) gây TLE. Đây là cải tiến then chốt, vì các bạn trong lớp đều thiết kế test nhắm vào quicksort.
- **Sort chỉ số:** bước merge chỉ chép `int` qua mảng `tmp`, **không bao giờ di chuyển chuỗi** — rẻ hơn cả swap O(1) của lần 1 (lần 1 vẫn phải tráo buffer của `string`). Xuất kết quả qua `a[idx[i]]`.
- **Ổn định** (dùng `<=`), bảo toàn thứ tự các chuỗi trùng.

### Bài C — Merge Sort trên chỉ số + comparator hai khóa

Dùng cùng merge sort theo chỉ số như bài B, với comparator `cmp` so độ dài trước rồi mới lexi.

- O(n·log n) đảm bảo — sống an toàn trong **TL 1 giây**, trong khi quicksort lần 1 có thể bị test đối kháng (cùng độ dài, reverse-lexi) đẩy lên O(n²) → TLE.
- Index sort tránh việc chép chuỗi dài 100 ký tự trong lúc merge (sort trực tiếp sẽ tốn tới O(n·log n·L) thao tác chép).
- Comparator chập mạch theo độ dài giữ chi phí mỗi phép so sánh ở mức thấp.

---

## Biên dịch và sử dụng

```bash
g++ -std=c++2b -O2 -o test_gen.o test_gen.cpp
./test_gen.o int 1 > test001.in
./test_gen.o strlexi 3 > test003.in
```
