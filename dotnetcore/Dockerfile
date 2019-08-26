FROM mcr.microsoft.com/dotnet/core/sdk:2.2 AS build
WORKDIR /app

# copy csproj and restore as distinct layers
COPY *.sln .
COPY UpdateNamesiloRecord/*.csproj ./UpdateNamesiloRecord/
RUN dotnet restore

# copy everything else and build app
COPY UpdateNamesiloRecord/. ./UpdateNamesiloRecord/
WORKDIR /app/UpdateNamesiloRecord
RUN dotnet publish -c Release -o out


FROM mcr.microsoft.com/dotnet/core/runtime:2.2 AS runtime
WORKDIR /app
COPY --from=build /app/UpdateNamesiloRecord/out ./
ENTRYPOINT ["dotnet", "UpdateNamesiloRecord.dll"]
