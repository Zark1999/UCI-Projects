package com.example.lab5.activity;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Toast;

import com.example.lab5.R;
import com.example.lab5.adapter.CategoryList_Adapter;
import com.example.lab5.object.Category;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class MainActivity extends AppCompatActivity {

//    Written by:
//        Name              Zirong Xu
//        Student netID     zirongx
//        StudentID         91574614
//
//    We made an app that works as an anime list;
//    The first page contains all the categories;
//    Click the specific category will load the second page which contains all the animes in that category
//    Click the specific anime will load the episodes stored in the anime, which could be clicked to nevigate to the website.
//
//    Long click will delete a category/anime/episode
//
//    The count number on every element in the list will update automatically
//
//    Reference: https://www.youtube.com/watch?v=EM2x33g4syY&list=PLk7v1Z2rk4hj6SDHf_YybDeVhUT9MXaj1&index=1
//
//    We have utilized three list views. The layouts are basically the same as well as the three activities.
//    The video taught us how to use listview and firebase and we entended the knowledge to build this app.

    public static final String CATEGORY_NAME = "CATEGORY_NAME";
    public static final String CATEGORY_ID = "CATEGORY_ID";

    EditText editText;
    Button button;
    ListView categoryListview;

    List<Category> categoryList;
    DatabaseReference databaseReference;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        editText = findViewById(R.id.editCategoryName);
        button = findViewById(R.id.addCategoryButton);
        categoryListview = findViewById(R.id.categories_listview);

        databaseReference = FirebaseDatabase.getInstance("https://lab5-f2989.firebaseio.com/").getReference("categories");
        categoryList = new ArrayList<>();

        button.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                addCategory();
            }
        });

        categoryListview.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Category category = categoryList.get(position);

                Intent intent = new Intent(MainActivity.this, AddAnimeActivity.class);

                intent.putExtra(CATEGORY_ID, category.getId());
                intent.putExtra(CATEGORY_NAME, category.getName());

                startActivity(intent);
            }
        });

        categoryListview.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
            @Override
            public boolean onItemLongClick(AdapterView<?> parent, View view, int position, long id) {
                Category category = categoryList.get(position);

                DatabaseReference anime_databaseReference = FirebaseDatabase.getInstance("https://lab5-f2989.firebaseio.com/").getReference("anime").child(category.getId());
                anime_databaseReference.removeValue();

                DatabaseReference episode_databaseReference = FirebaseDatabase.getInstance("https://lab5-f2989.firebaseio.com/").getReference("episodes").child(category.getId());
                episode_databaseReference.removeValue();

                databaseReference.child(category.getId()).removeValue();

                Toast.makeText(MainActivity.this, "Successfully deleted "+category.getName(), Toast.LENGTH_LONG).show();

                return true;
            }
        });

    }

    @Override
    protected void onStart() {
        super.onStart();

        databaseReference.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {

                categoryList.clear();

                for(DataSnapshot categorySnapShot : dataSnapshot.getChildren()){
                    Category category = categorySnapShot.getValue(Category.class);
                    categoryList.add(category);
                }

                Collections.sort(categoryList,new sortByName());

                CategoryList_Adapter categoryList_adapter = new CategoryList_Adapter(MainActivity.this, categoryList);
                categoryListview.setAdapter(categoryList_adapter);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });
    }

    private void addCategory(){
        String category_name = editText.getText().toString().trim();

        if (!category_name.isEmpty()){

            String id = databaseReference.push().getKey();

            Category category = new Category(id, category_name, 0);

            databaseReference.child(id).setValue(category);

            editText.setText("");

        } else {
            Toast.makeText(this, "Enter a category name", Toast.LENGTH_LONG).show();
        }
    }

    class sortByName implements Comparator<Category>{
        @Override
        public int compare(Category o1, Category o2) {
            return o1.getName().compareTo(o2.getName());
        }
    }

}
