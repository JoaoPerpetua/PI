
from django.shortcuts import render
from .models import Item, Records, CurrentState, Ware
from django.http import HttpResponse

from django_tables2 import SingleTableView

from .tables import ItemTable, RecordTable, CurrentStateTable, Waretable, Lastreadtable
from django.http import HttpResponseRedirect
from django.shortcuts import render
from .forms import FormItemForm

import logging

l = logging.getLogger('django.db.backends')
l.setLevel(logging.DEBUG)
l.addHandler(logging.StreamHandler())

DJANGO_TABLES2_TEMPLATE = "django_tables2/bootstrap4.html"


def index(request):
    return HttpResponse("SE FUNCIONAR ")


class ItemListView(SingleTableView):
    model = Item
    table_class = ItemTable
    template_name = 'UserEnd/item.html'


class RecordTableListView(SingleTableView):
    model = Records
    table_class = RecordTable
    template_name = 'UserEnd/records.html'


class CurrentStateListView(SingleTableView):
    model = CurrentState
    table_class = CurrentStateTable
    template_name = 'UserEnd/current_state.html'


def warehouse_item(request):

    currentitems = CurrentState.objects.raw('SELECT item.id, item.name, warehouse, item.mac, item.make, item.model, item.color FROM public.Current_state INNER JOIN public.item ON Current_state.MAC = item.MAC;')

    table1 = Waretable(currentitems)
    return render(request, 'UserEnd/currentitems.html', {"table": table1})


def item_lastreading(request):

    lastreading = CurrentState.objects.raw('SELECT DISTINCT ON (item.mac) item.id, item.name, warehouse, status, readingtime FROM public.records FULL OUTER JOIN public.item ON records.MAC = item.MAC order by item.mac, public.records.readingtime DESC;')

    table2 = Lastreadtable(lastreading)
    return render(request, 'UserEnd/lastreading.html', {"table": table2})


def item_form(request):

    if request.method == "POST":
        form = FormItemForm(request.POST or None)
        if form.is_valid():
                print(form.is_valid())


                print("post_data", form)
                name = form.cleaned_data['name']
                mac = form.cleaned_data['mac']
                make = form.cleaned_data['make']
                model = form.cleaned_data['model']
                color = form.cleaned_data['color']
                item = FormItemForm(request.POST)
                item.save()

                return HttpResponseRedirect('http://127.0.0.1:8000/userend/items')

    else:

        form = FormItemForm()

    return render(request, 'UserEnd/itemform.html',{"form": form})
