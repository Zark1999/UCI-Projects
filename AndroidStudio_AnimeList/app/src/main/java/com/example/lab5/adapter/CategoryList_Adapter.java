package com.example.lab5.adapter;

import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.example.lab5.object.Category;
import com.example.lab5.R;

import java.util.List;

public class CategoryList_Adapter extends ArrayAdapter<Category> {

//    Written by:
//        Name              Zirong Xu
//        Student netID     zirongx
//        StudentID         91574614

    private Activity context;
    private List<Category> categoryList;

    public CategoryList_Adapter(Activity context, List<Category> categoryList){
        super(context, R.layout.category_layout, categoryList);

        this.context = context;
        this.categoryList = categoryList;
    }

    @NonNull
    @Override
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        LayoutInflater inflater = context.getLayoutInflater();

        View listViewItem = inflater.inflate(R.layout.category_layout, null, true);

        TextView category_name = listViewItem.findViewById(R.id.category_name);
        TextView score = listViewItem.findViewById(R.id.category_count);

        Category category = categoryList.get(position);

        category_name.setText(category.getName());
        score.setText(String.valueOf(category.getCount()) + " anime in the list");

        return listViewItem;
    }
}
