#Client


در کل ،‌توابع موجود در کلاینت کار ساخت رشته ،‌ارسال رشته به سرور و دریافت رشته از آن ،‌نشان دادن اطلاعات در صفحه نمایشگر بر اساس اطلاعات دریافتی از سرور و ایجاد ارتباط با سرور را انجام می‌دهند.



تابع چت‌اپ :‌این تابع با استفاده از توابع کتابخانه‌ای شبکه اتصال به سرور را برقرار ساخته ،‌رشته ای به آن ارسال می‌کند و رشته ارسالی توسط سرور را دریافت می‌کند.در واقع اصل برنامه این تابع می‌باشد.


تابع منوی اکانت :‌با شروع برنامه صدا زده می‌شود.یوزرنیم و پسورد را دریافت کرده و با استفاده از توابع رشته،‌حالت مطلوب سرور را می‌سازد و با صدا زدن تابع چت‌اپ اتصال با سرور را برقرار می‌سازد.با دریافت رشته آن را پارس کرده و تشخیص می‌دهد که رجیستر و یا لاگین به درستی انجام شده است یا خیر.


تابع منوی اصلی :‌در صورت لاگین کردن این تابع صدا زده می‌شود.این تابع هم با دریافت با دریافت اطلاعات یک چنل ما را وارد آن چنل می‌کند.این تابع با استفاده از توابع کتابخانه‌ای رشته‌ای شبیه حالت مطلوب سرور می‌سازد.با صدا زدن تابع چت‌اپ اتصال با سرور برقرار شده ،رشته‌ی ساخته شده به سرور فرستاده میِ‌شود و یک رشته‌ از سرور به کلاینت می‌رسد.این رشته پارس شده و تشخیص داده می‌شود که آیا کانال به درستی ساخته شده و یا ما به درستی به چنل جوین شده‌ایم.در صورت انتخاب گزینه‌های اول و دوم و در دسترس بودن کانال تابع منوی چت صدا زده می‌شود.در صورت انتخاب گزینه‌ی سوم با صدا زده شدن تابع چت‌اپ و ایجاد ارتباط با سرور در صورت موفقیت آمیز بودن لاگ‌اوت تابع منوی اکانت صدا زده می‌شود.




تابع منوی چت :‌با انتخاب گزینه‌ی اول و وارد کردن پیام با استفاده ای توابع کتابخانه‌ای رشته‌ها رشته‌ی مطلوب سرور ساخته شده و با صدا زدن تابع چت‌اپ به سرور ارسال می‌شود.این پیام در دیتابیس سرور سیو می‌شود.رشته‌ی دریافتی پارس می‌شود و در صورت وجود خطا،خطای مربوطه چاپ می‌شود.با انتخاب گزینه دوم توسط کاربر با استفاده از توابع کتابخانه‌ای رشته‌ها رشته‌ی مطلوب سرور ساخته شده و با صدا زدن تابع چت‌اپ به سرور ارسال می شود.با دریافت رشته و پارس کردن آن محتویات آن به حالتی که مطلوب است چاپ می‌شود.گزینه‌ی سوم هم همانند دومی می‌باشد.با انتخاب گزینه‌ی چهارم طبق حالات قبل رشته ساخته  ، رشته ارسالی توسط سرور دریافت و در صورت موفقیت‌آمیز بودن پیغام مربوطه چاپ شده و تابع منوی اصلی صدا زده می شود.


همانطور که در اول ذکر شد بیشتر کار توابع کلاینت ساخت و پردازش رشته می باشد.


#Server


من سرور را فقط با یک تابع زدم و تقریبا تمام دستورات را در تابع مین گنجاندم.


خطوط اولیه‌ی برنامه مربوط به اتصال به سرور توسط توابع کتابخانه‌ای شبکه می‌باشد.که کارهای ساخت سوکت، اتصال سوکت و امثالهم را شامل می‌شود.


با صدا زدن تابع ریسیو رشته ارسالی توسط کلاینت دریافت می‌شود و با استفاده از تعدادی شرط چک می‌شود که کلاینت خواستار چیست.در صورتی که رشته با کلمه‌ی رجیستر شروع شود سرور با چک کردن یوزرنیم و پسورد در صورت وجود مشکل آن را در قالب جیسون برای کلاینت می‌فرستد و در صورت عدم وجود مشکل پیغام موفقیت را برای کلاینت ارسال می‌کند.ساخت جیسون با استفاده از توابع کتابخانه‌ای جیسون صورت می‌گیرد.با انجام فعل رجیستر فایلی با نام وارده توسط کاربر ساخه می‌شود که در آن پسورد فرد ذخیره می‌شود.این کارها هم با اسفاده از توابع کتابخانه‌ای فایل‌ها صورت می‌گیرد.


اگر رشته با لاگین شروع شود سرور با چک کردن یوزرنیم و پسورد با استفاده از دیتابیس خود پیغام لازم را برای کلاینت ارسال می‌کند.در این بخش تابع سازنده توکن صدا زده میِ‌شود و برای کاربر یک توکن می‌سازد که این توکن تا زمان لاگ‌اوت نشان‌دهنده‌ی این است که او در حال ارسال پیام به سرور می‌باشد.اسم و توکن فرد در استراکت ممبر ذخیره می‌شود.


در صورتی که رشته دریافتی با کریت چنل آغاز شود نام چنل چک می‌شود و در صورت مشکل ارور برای کلاینت ارسال می‌شود.اگر مشکلی نبود یک فایل با فرمت جیسون در دیتابیس ذخیره می‌شود.ضمنا پیام ساخت کانال توسط کاربری که آن را ساخته در فایل ذکر شده ذخیره می‌شود.


اگر رشته با جوین چنل آغاز شود نام کانال و نام فرد چک می‌شود که کانال ذکرشده موجود باشد و یوزر در کانال دیگری عضو نباشد.در صورت عدم وجود اشکال اطلاعات لازم در دیتابیس ذخیره می‌شود.با استفاده از توابع کتابخانه‌ای فایل پیام‌ها با وارد شدن پیغام عضویت عضو جدید آپدیت می‌شود.


اگر رشته با لاگ‌اوت آغاز شود نام فرد از اعضای آنلاین حذف شده و پیغام لازم به کلاینت ارسال می‌شود.در اینجا لازم است توکن فرد از بین برود.


اگر رشته با سند آغاز می‌شد سرور با پرینت فایل جیسون پیام‌ها با اضافه‌کردن پیغام جدید آن را آپدیت می‌کند و با ساخت یک جیسون و ارسال آن موفقیت‌آمیز بودن یا نبودن کار را به اطلاع کاربر می‌رساند.


در صورتی که رشته با رفرش یا چنل ممبر آغاز شود سرور پیام‌ها یا ممبرهای کانال را از دیتابیس درآورده و به صورت جیسون به کلاینت ارسال می‌کند.لازم به ذکر است در هر بار رفرش تنها پیام‌های جدید ارسال می‌شوند.این موضوع با اختصاص دادن یک عدد به هر کاربر و آپدیت آن در هر لاگین و جوین صورت می‌گیرد.


در صورتی که رشته با لیو آغاز شود سرور نام فرد را اعضای کانال و پیغام مربوطه را به صورت جیسون برای کلاینت ارسال می‌کند.


در هر مرحله اطلاعات وارده بعد از هر کلیدواژه چک می‌شوند و در صورت وجور هرگونه عدم تطابق ارور مربوطه تشخیص و برای کلاینت ارسال می‌شود.لازم به ذکر است تمام پیغام‌های ارسالی توسط سرور به کلاینت در قالب جیسون بوده و پس از ساخت جیسون مربوطه در یک پوینتر پرینت و سپس برای کلاینت ارسال می‌شود.