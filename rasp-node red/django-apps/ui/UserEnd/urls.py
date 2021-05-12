from django.urls import path

from . import views
from .views import ItemListView, RecordTableListView, CurrentStateListView, warehouse_item, item_form

from django.contrib.staticfiles.urls import staticfiles_urlpatterns
urlpatterns = [
	path('index', views.index, name='index'),
	path('items', ItemListView.as_view(), name='items'),
	path('records', RecordTableListView.as_view(), name='records'),
	path('current_state', CurrentStateListView.as_view(), name='current_state'),
	path('itemform', item_form),
	path('itemform/<int:id>/', item_form),
	path('currentitems', views.warehouse_item, name='currentitems'),
	path('lastreading', views.item_lastreading, name='lastreading'),

]
urlpatterns += staticfiles_urlpatterns('static/image')
