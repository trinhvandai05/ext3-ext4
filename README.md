Trịnh Văn Đại - 20233298

Trần Đức Hoàng - MSSV  

Đề tài 16: Tìm hiểu ext3/ext4 trên Linux và xây dựng chương trình dùng API để đọc metadata, inode/extent và vị trí block của file

Bài tập lớn gồm 2 phần
# Phần 1: Tìm hiểu về ext3/ext4
Lịch sử -> Tại sao cần ext -> ext3 giải quyết bài toán gì? -> ext4 giải quyết vấn đề gì mà ext3 không làm được? 
## Các khái niệm cần nắm
<Giải thích các khái niệm ở đây (metadata, inode, v.v.)>
## Đi sâu lý thuyết ext3
## Đi sâu lý thuyết ext4 

# Phần 2: Triển khai sử dụng API để đọc metadata, inode/extent, block của tệp
## 2.1: Sử dụng sẵn API của linux để đọc metadata, inode/extent, block của tệp
## 2.2: Tự dựng lại API tính từ thời điểm có ext2 và cần ext3
Trong code sử dụng sẵn API thì có 3 API quan trọng cần dựng lại để hiểu rõ vấn đề là stat(), statvfs() và ioclt()&FS_IOC_FIEMAP 
### 2.2.1: stat()
input: path của file 

các trả về: size, inode, uid, gid, mode, link, mtime 


