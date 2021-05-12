import django_tables2 as tables
from .models import Item, Records, CurrentState, Ware, Lastread


class ItemTable(tables.Table):
    class Meta:
        model = Item
        template_name = "django_tables2/bootstrap4.html"
        fields = ("id", "name", "make", "model", "color", "mac")


class RecordTable(tables.Table):
    class Meta:
        model = Records
        template_name = "django_tables2/bootstrap4.html"
        fields = ("id", "mac", "status", "warehouse", "readingtime")


class CurrentStateTable(tables.Table):
    class Meta:
        model = CurrentState
        template_name = "django_tables2/bootstrap4.html"
        fields = ("id", "mac", "warehouse", "readingtime")


class Waretable(tables.Table):
    class Meta:
        model = Ware
        template_name = "django_tables2/bootstrap4.html"
        fields = ("id","name", "warehouse", "mac", "make", "model", "color", "readingtime")


class Lastreadtable(tables.Table):
    class Meta:
        model = Lastread
        template_name = "django_tables2/bootstrap4.html"
        fields = ("id","name", "warehouse", "status", "readingtime")



