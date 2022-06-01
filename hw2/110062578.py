# -*- coding: utf-8 -*-
"""baseline85.ipynb

Automatically generated by Colaboratory.

Original file is located at
    https://colab.research.google.com/drive/1gsMqs974h513r6mvYKjV1aUzK75CllO9
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from google.colab import drive
drive.mount('/content/drive')

# 為了處理方便，把 'train.csv' 和 'test.csv' 合併起來，'test.csv'的 Weather 欄位用 0 補起來。
df = pd.read_csv('/content/drive/MyDrive/Data_Science/hw2/train.csv')
df_test = pd.read_csv('/content/drive/MyDrive/Data_Science/hw2/test.csv')
df_test['Label'] = np.zeros((len(df_test),))

# from sklearn.preprocessing import OneHotEncoder
# onehotencoder = OneHotEncoder(categorical_features = ["Loc"])

# for i, row in df.iterrows():
#     df.at[i,'humid'] = (df.at[i,'DayHumidity'] + df.at[i, 'NightHumidity']) / 2
# df = df.drop(columns=["DayHumidity", "NightHumidity"])



# 以 train_end_idx 作為 'train.csv' 和 'test.csv' 分界列，
train_end_idx = len(df)
df = pd.concat([df, df_test], sort=False)

# print(df.dtypes)

from sklearn.preprocessing import LabelEncoder
# le.fit(data['name'])
# le_name_mapping = dict(zip(le.classes_, le.transform(le.classes_)))
# print(le_name_mapping)
# {'Tom': 0, 'Nick': 1, 'Kate': 2}

labelencoder = LabelEncoder()
df['Date'] = labelencoder.fit_transform(df['Date'])
df['Loc'] = labelencoder.fit_transform(df['Loc'])
df['WindDir'] = labelencoder.fit_transform(df['WindDir'])
df['DayWindDir'] = labelencoder.fit_transform(df['DayWindDir'])
df['NightWindDir'] = labelencoder.fit_transform(df['NightWindDir'])

# from sklearn.preprocessing import OneHotEncoder
# onehotencoder = OneHotEncoder
df_ohe_df = pd.get_dummies(df['Loc'], prefix='Loc', drop_first=True)
df = pd.concat([df, df_ohe_df], axis=1)
# print(df_ohe_df.head(100))
df = df.drop(columns = ['Loc'])
# print(df.dtypes)

# print(df.head(100))

# 將非數值欄位拿掉
# df = df.drop(columns = [col for col in df.columns if df[col].dtype == np.object])
# print(df.dtypes)

# 將 missing value 補 0
df = df.fillna(df.median())

# print(df.head(10))

"""# train"""

from sklearn.model_selection import train_test_split

X_train, X_val, y_train, y_val = train_test_split(
    df.drop(columns = ['Label']).values[:train_end_idx, :],
    df['Label'].values[:train_end_idx], test_size=0.35)

X_test = df.drop(columns = ['Label']).values[train_end_idx:, :]

# from sklearn.decomposition import PCA
# pca = PCA(n_components=20)
# pca.fit(X_train)
# X_train = pca.transform(X_train)
# pca.fit(X_test)
# X_test = pca.transform(X_test)
# pca.fit(X_val)
# X_val = pca.transform(X_val)
# pca.fit(y_train)
# y_train = pca.transform(y_train)
# # print(len(df.columns))
# # df.head(100)

from sklearn.preprocessing import StandardScaler

sc = StandardScaler()
sc.fit(X_train)
X_train_std = sc.transform(X_train)
X_test_std = sc.transform(X_test)
X_val_std = sc.transform(X_val)

from imblearn.over_sampling import RandomOverSampler
ros = RandomOverSampler(random_state=0)
X_train_std, y_train = ros.fit_resample(X_train_std, y_train)

from sklearn.metrics import accuracy_score, f1_score
# from sklearn.tree import DecisionTreeClassifier
# #train tree model
# model = DecisionTreeClassifier()
# model.fit(X_train_std,y_train)

# from sklearn.svm import SVC
# model = SVC(kernel='rbf', probability=True)

# from sklearn.ensemble import RandomForestClassifier
# model = RandomForestClassifier(n_estimators=100, random_state=0)

# from sklearn.neighbors import KNeighborsClassifier
# model = KNeighborsClassifier(n_neighbors=5)

# from sklearn.linear_model import LinearRegression
# model = LinearRegression()

# from sklearn.naive_bayes import GaussianNB
# model = GaussianNB()

from sklearn.ensemble import AdaBoostClassifier
model = AdaBoostClassifier(n_estimators=500, random_state=0)

model.fit(X_train_std,y_train)

#predict
y_pred_decision = model.predict(X_val_std)
# print(y_val[:100])
# for i in range(100):
#   if y_val[i] != y_pred_decision[i]:
#     print(i)
print('Accuracy: %f' % accuracy_score(y_val, y_pred_decision))
print('f1-score: %f' % f1_score(y_val, y_pred_decision))

ans_pred = model.predict(X_test_std)
df_sap = pd.DataFrame(ans_pred.astype(int), columns = ['Label'])
df_sap.to_csv('myAns.csv',  index_label = 'Id')